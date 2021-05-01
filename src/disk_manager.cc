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
  wal_file_ = StringConcat(db_base, "memtable.log");
  InitFileIOImpl(&log_io_, wal_file_);
  sstable_file_ = StringConcat(db_base, "sstable.db");
  InitFileIOImpl(&db_io_, sstable_file_);
  temp_sstable_ = StringConcat(db_base, "sstable.temp");
  InitFileIOImpl(&temp_io_, temp_sstable_);
}

int DiskManager::GetFileSize(const string &filename) {
  struct stat stat_buf;
  int rc = stat(filename.c_str(), &stat_buf);
  return rc == 0 ? static_cast<int>(stat_buf.st_size) : -1;
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
void DiskManager::WriteLog(const char *log_data, int size, bool is_delete) {
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

void DiskManager::WriteDb(const char *db_data, int size) {
  assert(db_data != nullptr);
  assert(temp_io_.is_open());
  if (size == 0) {
    return;
  }
  temp_io_.seekp(0);
  WriteFileImpl(&temp_io_, db_data, size);
}

void DiskManager::WriteDb(const char *db_data, int size, int page_id) {
  assert(db_data != nullptr);
  assert(db_io_.is_open());
  if (size == 0) {
    return;
  }
  db_io_.seekp(page_id * PAGE_SIZE);
  WriteFileImpl(&db_io_, db_data, size);
}

void DiskManager::WriteFileImpl(fstream *io, const char *data, int size) {
  io->write(data, size);
  if (io->bad()) {
    LOG_ERROR("I/O error while writing data");
  }
  io->flush();
}

void DiskManager::ReadLog(char *page, int size) {
  assert(page != nullptr);
  assert(log_io_.is_open());
  if (GetFileSize(wal_file_) == 0) {
    return;
  }
  log_io_.seekp(0);
  ReadFileImpl(&log_io_, page, size);
}

void DiskManager::ReadDb(char *page) {
  assert(page != nullptr);
  assert(temp_io_.is_open());
  if (GetFileSize(temp_sstable_) == 0) {
    return;
  }
  temp_io_.seekp(0);
  ReadFileImpl(&temp_io_, page, PAGE_SIZE);
}

void DiskManager::ReadDb(char *page, int page_id) {
  assert(page != nullptr);
  assert(db_io_.is_open());
  int file_offset = page_id * PAGE_SIZE;
  if (file_offset >= GetFileSize(sstable_file_)) {
    return;
  }
  db_io_.seekp(file_offset);
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
