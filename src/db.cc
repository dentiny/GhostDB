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
  levels_(vector<unique_ptr<Level*>>(MAX_LEVEL_NUM)) {
  LOG_DEBUG("Open db at ", db_base);
}

bool GhostDB::Put(int32_t key, int32_t val) {
  if (buffer_->Put(key, val)) {
    LOG_DEBUG("Place key:", key, ", value:", val, " in memtable");
    return true;
  }
  LOG_DEBUG("memtable full, dump memtable into SSTable");
  return false;
}

bool GhostDB::Get(int32_t key, int32_t *val) {
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

bool GhostDB::Delete(int32_t key) {
  return Put(key, TOMBSTOME);
}

GhostDB::~GhostDB() {
  LOG_DEBUG("~GhostDB");
}

}  // namespace ghostdb
