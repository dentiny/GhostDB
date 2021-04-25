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

#include <string>

#include "db.h"

using std::make_unique;
using std::unique_ptr;
using std::vector;

namespace ghostdb {

GhostDB::GhostDB() :
  buffer_(make_unique<Buffer>(MAX_BUFFER_SIZE)),
  levels_(vector<unique_ptr<Level>>(MAX_LEVEL_NUM)) {
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
  if (levels_[0] == nullptr) {
    levels_[0] = make_unique<Level>(0 /* level */);
  }
  bool dump_suc = levels_[0]->DumpTable(buffer_->kv_);
  if (dump_suc) {
    LOG_DEBUG("Dump memtable in the first level");
  } else {
    LOG_DEBUG("First level has been full, compaction needed");
  }
  return false;
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
