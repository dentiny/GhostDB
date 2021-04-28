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
#include "page.h"
#include "util.h"

using std::map;
using std::string;

namespace ghostdb {

DiskManager::DiskManager() {
  db_file_ = StringConcat(db_base, "memtable.log");
  log_io_.open(db_file_, std::ios::binary | std::ios::trunc | std::ios::out);
  log_io_.close();
  // reopen with original mode
  log_io_.open(db_file_, std::ios::binary | std::ios::in | std::ios::app | std::ios::out);
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
  db_file_ = StringConcat(db_base, filename);
  LOG_DEBUG("DB file:", db_file_);
  InitDbFileImpl();
}

DiskManager::DiskManager(const string& filename) {
  LOG_DEBUG("Temporary file for compaction:", filename);
  db_file_ = filename;
  InitDbFileImpl();
}

void DiskManager::InitDbFileImpl() {
  db_io_.open(db_file_, std::ios::binary | std::ios::trunc | std::ios::out);
  db_io_.close();
  // reopen with original mode
  db_io_.open(db_file_, std::ios::binary | std::ios::in | std::ios::app | std::ios::out);
  if (!db_io_.is_open()) {
    LOG_ERROR("cannot open db file");
  }
}

// Compaction would remove SSTable files, and should be reinitialized when Run re-access them.
void DiskManager::ReinitDbFile() {
  LOG_DEBUG("Reinitialize db file ", db_file_);
  InitDbFileImpl();
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
  assert(log_io_.is_open());
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

void DiskManager::WriteDb(char *db_data, int size) {
  assert(db_data != nullptr);
  assert(db_io_.is_open());
  if (size == 0) {
    return;
  }
  db_io_.seekg(0, db_io_.beg);  // reset file offset to the beginning
  db_io_.write(db_data, size);

  // check for I/O error
  if (db_io_.bad()) {
    LOG_ERROR("I/O error while writing log");
    return;
  }
  // needs to flush to keep disk file in sync
  db_io_.flush();
}

// Read at the page granularity.
void DiskManager::ReadDb(Bloom *filter, memtable_t *memtable) {
  assert(db_io_.is_open());
  char *page_data = new char[PAGE_SIZE];  // NOTE: page_data will be de-allocated within Page::Page
  db_io_.seekg(0, db_io_.beg);  // reset file offset to the beginning
  db_io_.read(page_data, PAGE_SIZE);
  if (db_io_.bad()) {
    LOG_ERROR("I/O error while reading db data");
  }
  Page page(page_data);
  page.GetMemtable(filter, memtable);
}

void DiskManager::RemoveTable() {
  assert(db_io_.is_open());
  db_io_.close();
  RemoveFile(db_file_.c_str());
}

}  // namespace ghostdb
