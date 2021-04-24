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

using std::vector;

namespace ghostdb {

GhostDB::GhostDB() :
  buffer_(nullptr),
  levels_(vector<Level*>(MAX_LEVEL_NUM, nullptr)) {}

bool GhostDB::Open(const char *path, GhostDB **db) {
  LOG_DEBUG("Open");
  return true;
}

bool GhostDB::Put(int32_t key, int32_t val) {
  LOG_DEBUG("Put");
  return true;
}

bool GhostDB::Get(int32_t key, int32_t *val) {
  LOG_DEBUG("Get");
  return true;
}

bool GhostDB::Delete(int32_t key) {
  LOG_DEBUG("Delete");
  return true;
}

GhostDB::~GhostDB() {
  LOG_DEBUG("~GhostDB");
}

}  // namespace ghostdb
