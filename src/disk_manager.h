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
 * Three sources of DiskManager IO:
 * (1) buffer WAL, held within Buffer and initialized by DiskManager(), bound with memtable.log
 * (2) SSTable IO, held within Run and initialized by DiskManager(level, run), bound with one specific level_0_run_0.db
 * (3) compaction IO, held within CompactionManager and initialized by DiskManager(is_compaction = true), could update
 * related IO stream 
 */

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
  DiskManager(bool is_compaction = false);  // memtable WAL
  DiskManager(int level, int run);  // SSTable
  ~DiskManager() noexcept;
  void WriteLog(char *log_data, int size);
  /** DB data consists of bloom filter and key-value pairs */
  void AppendDb(const char *db_data, int size);
  void FlushDb();

 private:
  bool is_compaction_;  // only compaction thread could set db_io_
  std::fstream log_io_;
  std::fstream db_io_;  // SSTable
};

}  // namespace ghostdb
