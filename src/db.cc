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
#include "util.h"

using std::fstream;
using std::make_unique;
using std::unique_ptr;
using std::vector;

namespace ghostdb {

GhostDB::GhostDB() {
  InitDirectory(db_base);  // make sure db_base exists at the very beginning of set-up
  LOG_DEBUG("Open db at ", db_base);
  disk_manager_ = make_unique<DiskManager>();
  buffer_ = make_unique<Buffer>(MAX_BUFFER_SIZE, disk_manager_.get());
  sstable_manager_ = make_unique<SSTableManager>(disk_manager_.get());
  compaction_manager_ = make_unique<CompactionManager>(sstable_manager_.get());
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
  buffer_->log_manager_->Flush();
  while (!sstable_manager_->DumpSSTable(buffer_->kv_)) {
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

GhostDB::~GhostDB() {
  LOG_DEBUG("Exit GhostDB, place buffer into SSTable");
  while (!sstable_manager_->DumpSSTable(buffer_->kv_)) {
    compaction_manager_->RequestMinorCompaction();
  }
}

}  // namespace ghostdb
