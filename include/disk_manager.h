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
 * owners of DiskManager:
 * (1) LogManager and RecoveryManager: initialized by DiskManager(), working on buffer WAL
 * (2) Run: each Run holds a DiskManager to manage its corresponding SSTable, initialized by DiskManager(level, run)
 */

#pragma once

#include <cstdint>
#include <cstring>
#include <fstream>
#include <map>
#include <string>

#include "common.h"
#include "config.h"
#include "logger.h"

namespace ghostdb {

class DiskManager {
 public:
  DiskManager();  // memtable WAL
  DiskManager(int level, int run);  // SSTable
  ~DiskManager() noexcept;
  void WriteLog(char *log_data, int size);
  void WriteDb(char *db_data, int size);
  void LoadTable(memtable_t *memtable);

 private:
  std::fstream log_io_;
  std::fstream db_io_;  // SSTable
};

}  // namespace ghostdb
