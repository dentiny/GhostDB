//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// db.cc
//
// Identification: src/db.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <string>

#include "db.h"
#include "recovery_manager.h"
#include "util.h"

using std::fstream;
using std::lower_bound;
using std::make_pair;
using std::make_unique;
using std::unique_ptr;
using std::vector;

namespace ghostdb {

GhostDB::GhostDB() {
  InitDirectory(db_base);  // make sure db_base exists at the very beginning of set-up
  LOG_DEBUG("Open db at ", db_base);
  disk_manager_ = make_unique<DiskManager>();
  sstable_manager_ = make_unique<SSTableManager>(disk_manager_.get());
  compaction_manager_ = make_unique<CompactionManager>(sstable_manager_.get());
  RecoveryManager recovery_manager(disk_manager_.get(), sstable_manager_.get(), compaction_manager_.get());
  recovery_manager.PerformRecover();
  buffer_ = make_unique<Buffer>(MAX_BUFFER_SIZE, disk_manager_.get());
}

/*
 * 1. Place in buffer if possible.
 * 2. If buffer already been full(WAL size == MAX_BUFFER_SIZE)
 *  (1) flush WAL into disk
 *  (2) dump buffer into first level
 *  (3) re-place key-value pair into the buffer
 */
bool GhostDB::Put(Key key, Val val) {
  if (buffer_->Put(key, val)) {
    LOG_DEBUG("Place key:", key, ", value:", val, " in memtable");
    return true;
  }

  LOG_DEBUG("memtable full, dump memtable into SSTable");
  buffer_->FlushWAL();
  while (!sstable_manager_->DumpSSTable(buffer_->kv_)) {
    compaction_manager_->RequestMinorCompaction();
  }
  disk_manager_->WriteLog(zero_data, PAGE_SIZE, true /* is_delete */);
  buffer_->ClearKV();
  Put(key, val);
  return true;
}

bool GhostDB::Get(Key key, Val *val) {
  if (buffer_->Get(key, &val)) {
    if (val == nullptr) {
      LOG_DEBUG("key:", key, " has been deleted");
      return false;
    }
    LOG_DEBUG("key is stored at memtable");
    return true;
  }

  LOG_DEBUG("key is not stored at memtable, search at SSTable");
  Bloom bloom_filter;
  sstable_t memtable;
  for (int level_no = 0; level_no < MAX_LEVEL_NUM; ++level_no) {
    int cur_max_run = (level_no + 1) * MAX_RUN_PER_LEVEL;
    for (int run_no = cur_max_run - 1; run_no >= 0; --run_no) {
      if (sstable_manager_->LoadSSTable(level_no, run_no, &bloom_filter, &memtable)) {
        if (bloom_filter.CouldHasKey(key)) {
          LOG_DEBUG("key could exists at level:", level_no, ", run:", run_no);
          auto target = make_pair(key, TOMBSTOME);
          auto it = lower_bound(memtable.begin(), memtable.end(), target);
          if (it->first == key) {  // locate the target key
            if (it->second == TOMBSTOME) {
              LOG_DEBUG("key:", key, " has been deleted");
              return false;
            } else {
              LOG_DEBUG("get mapped value:", it->second);
              *val = it->second;
              return true;
            }
          }
        } else {
          LOG_DEBUG("key couldn't exists at level:", level_no, ", run:", run_no);
        }
      }
    }
  }
  return false;
}

bool GhostDB::Delete(Key key) {
  return Put(key, TOMBSTOME);
}

void GhostDB::GetRange(Key key1, Key key2, buffer_t *res) {
  assert(key1 <= key2);
  buffer_->GetRange(key1, key2, res);
  Bloom bloom_filter;
  sstable_t memtable;
  auto target1 = make_pair(key1, TOMBSTOME);
  auto target2 = make_pair(key2, TOMBSTOME);
  for (int level_no = 0; level_no < MAX_LEVEL_NUM; ++level_no) {
    int cur_max_run = (level_no + 1) * MAX_RUN_PER_LEVEL;
    for (int run_no = cur_max_run - 1; run_no >= 0; --run_no) {
      if (sstable_manager_->LoadSSTable(level_no, run_no, &bloom_filter, &memtable)) {
        const auto& kv1 = memtable.front();
        const auto& kv2 = memtable.back();
        if (kv1.first > key2 || kv2.first < key1) {
          continue;
        }
        auto it1 = lower_bound(memtable.begin(), memtable.end(), target1);
        auto it2 = lower_bound(memtable.begin(), memtable.end(), target2);
        if (it2 != memtable.end()) {
          ++it2;
        }
        for (auto it = it1; it != it2; ++it) {
          Key key = it->first;
          Val val = it->second;
          if (res->find(key) == res->end()) {
            (*res)[key] = val;
          }
        }
      }
    }
  }
}

/*
 * 1. Dump buffer into WAL, then SSTable file.
 * 2. Perform major compaction at destructor at ~CompactionManager.
 * 3. Clear WAL file, considering RecoveryManager.
 */
GhostDB::~GhostDB() {
  LOG_DEBUG("Exit GhostDB, place buffer into SSTable");
  buffer_->FlushWAL();
  while (!sstable_manager_->DumpSSTable(buffer_->kv_)) {
    compaction_manager_->RequestMinorCompaction();
  }
  disk_manager_->WriteLog(zero_data, PAGE_SIZE, true /* is_delete */);
}

}  // namespace ghostdb
