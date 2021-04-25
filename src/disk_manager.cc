//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// disk_manager.cc
//
// Identification: src/disk_manager.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include <cassert>

#include "disk_manager.h"
#include "string_util.h"

using std::string;

namespace ghostdb {

DiskManager::DiskManager() {
  string log_file = StringConcat(db_base, "memtable.log");
  log_io_.open(log_file, std::ios::binary | std::ios::trunc | std::ios::out);
  log_io_.close();
  // reopen with original mode
  log_io_.open(log_file, std::ios::binary | std::ios::in | std::ios::app | std::ios::out);
  if (!log_io_.is_open()) {
    LOG_ERROR("cannot open db log");
  }
}

DiskManager::DiskManager(int level, int run) {
  LOG_DEBUG("run = ", run, "; level = ", level);
  assert(level < MAX_LEVEL_NUM);
  assert(run < (level + 1) * MAX_RUN_PER_LEVEL);
  char filename[25] = { 0 };
  snprintf(filename, sizeof(filename), "level_%d_run_%d.db", level, run);
  string db_file = StringConcat(db_base, filename);
  LOG_DEBUG("DB file:", db_file);
  db_io_.open(db_file, std::ios::binary | std::ios::trunc | std::ios::out);
  db_io_.close();
  // reopen with original mode
  db_io_.open(db_file, std::ios::binary | std::ios::in | std::ios::app | std::ios::out);
  if (!db_io_.is_open()) {
    LOG_ERROR("cannot open db file");
  }
}

DiskManager::~DiskManager() {
  if (log_io_.is_open()) {
    log_io_.close();
  }
  if (db_io_.is_open()) {
    db_io_.close();
  }
}

void DiskManager::WriteLog(char *log_data, int size) {
  assert(log_data != nullptr);
  if (size == 0) {
    return;
  }
  log_io_.write(log_data, size);

  // check for I/O error
  if (log_io_.bad()) {
    LOG_ERROR("I/O error while writing log");
    return;
  }
  // needs to flush to keep disk file in sync
  log_io_.flush();
}

void DiskManager::AppendDb(const char *db_data, int size) {
  assert(db_data != nullptr);
  if (size == 0) {
    return;
  }
  db_io_.write(db_data, size);

  // check for I/O error
  if (db_io_.bad()) {
    LOG_ERROR("I/O error while writing log");
    return;
  }
}

void DiskManager::FlushDb() {
  // needs to flush to keep disk file in sync
  db_io_.flush();
}

}  // namespace ghostdb
