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
  buffer_(make_unique<Buffer>(MAX_BUFFER_SIZE)),
  sstable_manager_(make_unique<SSTableManager>()),
  compaction_manager_(make_unique<CompactionManager>()) {
  InitDirectory(db_base);
  LOG_DEBUG("Open db at ", db_base);
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
  while (!sstable_manager_->DumpTable(buffer_->kv_)) {
    compaction_manager_->RequestMinorCompaction();
    break;
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
  LOG_DEBUG("~GhostDB");
}

}  // namespace ghostdb
