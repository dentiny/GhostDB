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

#include "config.h"
#include "logger.h"

namespace ghostdb {

class Buffer {
 public:
  Buffer(uint32_t buffer_size);
  bool Put(int32_t key, int32_t val);
  bool Get(int32_t key, int32_t *val);
  bool Delete(int32_t key);

 private:
  uint32_t buffer_size_;
  std::map<int32_t, int32_t> kv_;
};

}  // namespace ghostdb
