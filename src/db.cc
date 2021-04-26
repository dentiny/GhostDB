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

#include <cstdio>
#include <fstream>
#include <string>

#include "db.h"
#include "fs_util.h"

using std::fstream;
using std::make_unique;
using std::unique_ptr;
using std::vector;

namespace ghostdb {

GhostDB::GhostDB() :
  compaction_manager_(make_unique<CompactionManager>()),
  buffer_(make_unique<Buffer>(MAX_BUFFER_SIZE)),
  levels_(vector<unique_ptr<Level>>(MAX_LEVEL_NUM)) {
  InitDirectory(db_base);
  LOG_DEBUG("Open db at ", db_base);
}

/*
 * 1. Place in buffer if possible.
 * 2. If buffer already been full(WAL size == MAX_BUFFER_SIZE)
 *  (1) flush WAL into disk
 *  (2) dump buffer into first level
 */
bool GhostDB::Put(Key key, Val val) {
  if (buffer_->Put(key, val)) {
    LOG_DEBUG("Place key:", key, ", value:", val, " in memtable");
    return true;
  }

  LOG_DEBUG("memtable full, dump memtable into SSTable");
  buffer_->log_manager_->Flush();
  int level_no = -1;
  int run_no = -1;
  bool can_dump = GetAvaiRun(&level_no, &run_no);
  if (can_dump) {
    if (levels_[level_no] == nullptr) {
      levels_[level_no] = make_unique<Level>(level_no);
      run_no = 0;
    }
    levels_[level_no]->DumpTable(run_no, buffer_->kv_);
  } else {
    compaction_manager_->RequestMinorCompaction();
  }
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
  return false;
}

bool GhostDB::Delete(Key key) {
  return Put(key, TOMBSTOME);
}

/*
 * Merge down policy:
 * (1) Search from bottom to top
 * (2) Search from left to right
 *
 * @param[out] level_no: only meaningful when return true
 * @param[out] run_no: only meaningful when return true, -1 if level not allocated yet
 * @return: whether there's available run
 */
bool GhostDB::GetAvaiRun(int *level_no, int *run_no) const {
  for (int idx = MAX_LEVEL_NUM - 1; idx >= 0; --idx) {
    if (levels_[idx] == nullptr) {
      *level_no = idx;
      *run_no = -1;
      LOG_DEBUG("Could persist to empty level ", *level_no);
      return true;
    }
    int avai_run = levels_[idx]->GetAvaiRun();
    if (avai_run != -1) {
      *level_no = idx;
      *run_no = avai_run;
      LOG_DEBUG("Could persist to level ", *level_no, ", run ", *run_no);
      return true;
    }
  }
  LOG_DEBUG("No available run, compaction needed");
  return false;
}

GhostDB::~GhostDB() {
  LOG_DEBUG("~GhostDB");
}

}  // namespace ghostdb
