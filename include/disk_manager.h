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

#include "common.h"
#include "config.h"
#include "logger.h"

namespace ghostdb {

class DiskManager {
 public:
  DiskManager();
  ~DiskManager() noexcept;
  void WriteLog(char *log_data, int size, bool is_delete = false);
  void WriteDb(char *db_data, int size);
  void WriteDb(char *db_data, int size, int level, int run);
  void ReadLog(char *log_data, int size);
  void ReadDb(char *page_data);
  void ReadDb(char *page_data, int level, int run);

 private:
  void InitFileIOImpl(std::fstream *io, const std::string& filename);
  void WriteFileImpl(std::fstream *io, char *data, int size);
  void ReadFileImpl(std::fstream *io, char *data, int size);

 private:
  std::fstream log_io_;  // WAL
  std::fstream db_io_;  // SSTable
  std::fstream temp_io_;  // temporary file for compaction
};

}  // namespace ghostdb
