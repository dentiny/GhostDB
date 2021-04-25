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

#pragma once

#include <cstdint>
#include <cstring>
#include <fstream>
#include <map>
#include <string>

#include "config.h"
#include "logger.h"

namespace ghostdb {

class DiskManager {
 public:
  DiskManager();  // memtable WAL
  DiskManager(int level, int run);  // SSTable
  ~DiskManager() noexcept;
  void WriteLog(char *log_data, int size);
  /** DB data consists of bloom filter and key-value pairs */
  void AppendDb(const char *db_data, int size);
  void FlushDb();

 private:
  std::fstream log_io_;
  std::fstream db_io_;  // SSTable
};

}  // namespace ghostdb
