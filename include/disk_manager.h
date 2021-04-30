//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// disk_manager.h
//
// Identification: src/disk_manager.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

/*
 * Lessons from C++ fstream:
 * (1) Never use std::ios::app as open mode, it doesn't work for file offset updating
 * (2) Before reading file, make sure offset is less than file size
 * (3) For setting file offset, use fio.seekp()
 */

#pragma once

#include <cstdint>
#include <cstring>
#include <fstream>
#include <map>
#include <sys/stat.h>

#include "common.h"
#include "config.h"
#include "logger.h"

namespace ghostdb {

class DiskManager {
 public:
  DiskManager();
  ~DiskManager() noexcept;
  void WriteLog(const char *log_data, int size, bool is_delete = false);
  void WriteDb(const char *db_data, int size);
  void WriteDb(const char *db_data, int size, int page_id);
  
  void ReadLog(char *log_data, int size);
  void ReadDb(char *page_data);
  void ReadDb(char *page_data, int page_id);

 private:
  int GetFileSize(const std::string &filename);
  void InitFileIOImpl(std::fstream *io, const std::string& filename);
  void WriteFileImpl(std::fstream *io, const char *data, int size);
  void ReadFileImpl(std::fstream *io, char *data, int size);

 private:
  std::string wal_file_;  // WAL
  std::string sstable_file_;  // SSTable
  std::string temp_sstable_;  // temporary file for compaction
  std::fstream log_io_;  // WAL
  std::fstream db_io_;  // SSTable
  std::fstream temp_io_;  // temporary file for compaction
};

}  // namespace ghostdb
