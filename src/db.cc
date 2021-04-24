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

#include "db.h"

using namespace std;

namespace ghostdb {

GhostDB::GhostDB() :
  buffer_(make_unique<Buffer>(MAX_BUFFER_SIZE)),
  levels_(vector<Level*>(MAX_LEVEL_NUM, nullptr)) {}

bool GhostDB::Open(const char *path, GhostDB **db) {
  LOG_DEBUG("Open");
  return true;
}

bool GhostDB::Put(int32_t key, int32_t val) {
  if (buffer_->Put(key, val)) {
    return true;
  }
  return true;
}

bool GhostDB::Get(int32_t key, int32_t *val) {
  if (buffer_->Get(key, val)) {
    return true;
  }
  return false;
}

bool GhostDB::Delete(int32_t key) {
  if (buffer_->Delete(key)) {
    return true;
  }
  return false;
}

GhostDB::~GhostDB() {
  LOG_DEBUG("~GhostDB");
}

}  // namespace ghostdb
