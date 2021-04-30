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
#include "util.h"

using std::fstream;
using std::string;

namespace ghostdb {

DiskManager::DiskManager() {
  string wal_file = StringConcat(db_base, "memtable.log");
  InitFileIOImpl(&log_io_, wal_file);
  string sstable_file = StringConcat(db_base, "sstable.db");
  InitFileIOImpl(&db_io_, sstable_file);
  string temp_sstable = StringConcat(db_base, "sstable.temp");
  InitFileIOImpl(&temp_io_, temp_sstable);
}

void DiskManager::InitFileIOImpl(fstream *io, const string& filename) {
  LOG_DEBUG("Initializing DB file:", filename);
  io->open(filename, std::ios::binary | std::ios::trunc | std::ios::out);
  io->close();
  // reopen with original mode
  io->open(filename, std::ios::binary | std::ios::in | std::ios::out);
  if (!io->is_open()) {
    LOG_ERROR("cannot open db file:", filename);
  }
}

// TODO: remove WAL and temporary SSTable for recovery
DiskManager::~DiskManager() {
  LOG_DEBUG("DiskManager::~DiskManager");
  if (log_io_.is_open()) {
    log_io_.close();
  }
  if (db_io_.is_open()) {
    db_io_.close();
  }
  if (temp_io_.is_open()) {
    temp_io_.close();
  }
}

// WAL'll be cleared after memtable is dumped into SSTable.
void DiskManager::WriteLog(char *log_data, int size, bool is_delete) {
  assert(log_data != nullptr);
  assert(log_io_.is_open());
  if (size == 0) {
    return;
  }
  if (is_delete) {
    log_io_.seekp(0);
  }
  WriteFileImpl(&log_io_, log_data, size);
  if (is_delete) {
    log_io_.seekp(0);
  }
}

void DiskManager::WriteDb(char *db_data, int size) {
  assert(db_data != nullptr);
  assert(temp_io_.is_open());
  if (size == 0) {
    return;
  }
  temp_io_.seekp(0);
  WriteFileImpl(&temp_io_, db_data, size);
}

void DiskManager::WriteDb(char *db_data, int size, int level, int run) {
  assert(db_data != nullptr);
  assert(db_io_.is_open());
  if (size == 0) {
    return;
  }
  db_io_.seekp(GetRunIndex(level, run) * PAGE_SIZE);
  WriteFileImpl(&db_io_, db_data, size);
}

void DiskManager::WriteFileImpl(fstream *io, char *data, int size) {
  io->write(data, size);
  if (io->bad()) {
    LOG_ERROR("I/O error while writing data");
  }
  io->flush();
}

void DiskManager::ReadLog(char *page, int size) {
  assert(page != nullptr);
  assert(log_io_.is_open());
  log_io_.seekg(0, log_io_.beg);  // reset file offset to the beginning
  ReadFileImpl(&log_io_, page, size);
}

void DiskManager::ReadDb(char *page) {
  assert(page != nullptr);
  assert(temp_io_.is_open());
  temp_io_.seekg(0, temp_io_.beg);  // reset file offset to the beginning
  ReadFileImpl(&temp_io_, page, PAGE_SIZE);
}

void DiskManager::ReadDb(char *page, int level, int run) {
  assert(page != nullptr);
  assert(db_io_.is_open());
  db_io_.seekp(GetRunIndex(level, run) * PAGE_SIZE);
  ReadFileImpl(&db_io_, page, PAGE_SIZE);
}

void DiskManager::ReadFileImpl(fstream *io, char *data, int size) {
  io->read(data, size);
  if (io->bad()) {
    LOG_ERROR("I/O error while reading data");
  }
  io->flush();
}

}  // namespace ghostdb
