//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// log_manager.h
//
// Identification: src/log_manager.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>

#include "config.h"
#include "disk_manager.h"

namespace ghostdb {

class LogManager {
 public:
  LogManager(int level, int run);
  ~LogManager() noexcept;
  void RunFlushThread();
  void StopFlushThread();
  void AppendLogRecord();

 private:
  std::unique_ptr<DiskManager> disk_manager_;
  char *log_buffer_;
  char *flush_buffer_;
};

}
