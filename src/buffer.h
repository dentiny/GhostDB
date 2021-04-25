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

#include "config.h"
#include "disk_manager.h"
#include "logger.h"

namespace ghostdb {

class Buffer {
 public:
  explicit Buffer(uint32_t buffer_size);
  bool Put(int32_t key, int32_t val);
  bool Get(int32_t key, int32_t **val);

 private:
  uint32_t buffer_size_;
  std::map<int32_t, int32_t> kv_;
  /** memtable WAL */
  std::unique_ptr<DiskManager> disk_manager_;
};

}  // namespace ghostdb
