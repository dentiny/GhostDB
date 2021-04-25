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

#include <cstring>
#include <fstream>
#include <string>

#include "logger.h"

namespace ghostdb {

class DiskManager {
 public:
  DiskManager();  // memtable WAL
  DiskManager(int level, int run);  // SSTable
  ~DiskManager() noexcept;
  void WriteLog(char *log_data, int size);
  void WriteDb();

 private:
  std::fstream log_io_;
  std::fstream db_io_;  // SSTable
};

}  // ghostdb
