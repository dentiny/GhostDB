//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// buffer.h
//
// Identification: src/buffer.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "common.h"
#include "config.h"
#include "disk_manager.h"
#include "logger.h"
#include "log_manager.h"

namespace ghostdb {

class GhostDB;

class Buffer {
  friend class GhostDB;

 public:
  explicit Buffer(uint32_t buffer_size, DiskManager *disk_manager);
  bool Put(Key key, Val val);
  bool Get(Key key, Val **val);
  void GetRange(Key key1, Key key2, buffer_t *res);  // inclusive
  void ClearKV() { kv_.clear(); }
  void FlushWAL() { log_manager_->Flush(); }

 private:
  uint32_t buffer_size_;
  buffer_t kv_;
  /** log_manager for buffer WAL */
  std::unique_ptr<LogManager> log_manager_;
};

}  // namespace ghostdb
