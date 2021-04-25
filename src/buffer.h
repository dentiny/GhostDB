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

/*
 * NOTE: Everytime a Put() is invoked, increment cur_size_ by 1, and append it to WAL.
 * Also, cur_size_ could be different with kv_.size(), which is # of valid kv pairs. 
 */

#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "config.h"
#include "logger.h"
#include "log_manager.h"

namespace ghostdb {

class Buffer {
 public:
  explicit Buffer(uint32_t buffer_size);
  bool Put(int32_t key, int32_t val);
  bool Get(int32_t key, int32_t **val);

 private:
  uint32_t cur_size_;
  uint32_t buffer_size_;
  std::map<int32_t, int32_t> kv_;
  /** log_manager for buffer WAL */
  std::unique_ptr<LogManager> log_manager_;
};

}  // namespace ghostdb
