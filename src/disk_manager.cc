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

namespace ghostdb {

DiskManager::DiskManager() {
  std::string log_file("memtable.log");
  log_io_.open(log_file, std::ios::binary | std::ios::in | std::ios::app | std::ios::out);
  // directory or file does not exist
  if (!log_io_.is_open()) {
    log_io_.clear();
    // create a new file
    log_io_.open(log_file, std::ios::binary | std::ios::trunc | std::ios::app | std::ios::out);
    log_io_.close();
    // reopen with original mode
    log_io_.open(log_file, std::ios::binary | std::ios::in | std::ios::app | std::ios::out);
    if (!log_io_.is_open()) {
      LOG_ERROR("cannot open db log");
    }
  }
}

DiskManager::DiskManager(int level, int run) {
  char filename[25] = { 0 };
  snprintf(filename, 25, "level_%d_run_%d.db", level, run);
  std::string db_file = std::string(filename);
  db_io_.open(db_file, std::ios::binary | std::ios::in | std::ios::out);
  // directory or file does not exist
  if (!db_io_.is_open()) {
    db_io_.clear();
    // create a new file
    db_io_.open(db_file, std::ios::binary | std::ios::trunc | std::ios::out);
    db_io_.close();
    // reopen with original mode
    db_io_.open(db_file, std::ios::binary | std::ios::in | std::ios::out);
    if (!db_io_.is_open()) {
      LOG_ERROR("cannot open db file");
    }
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

void DiskManager::WriteDb() {

}

}  // namespace ghostdb
