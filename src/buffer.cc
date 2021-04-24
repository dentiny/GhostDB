//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// buffer.cc
//
// Identification: src/buffer.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include "buffer.h"

namespace ghostdb {

Buffer::Buffer(uint32_t buffer_size) :
  buffer_size_(buffer_size) {}

bool Buffer::Put(int32_t key, int32_t val) {
  if (kv_.size() == buffer_size_ && kv_.find(key) == kv_.end()) {
    LOG_DEBUG("Buffer has been full.");
    return false;
  }
  kv_.emplace(key, val);
  return true;
}

bool Buffer::Get(int32_t key, int32_t *val) {
  if (kv_.find(key) != kv_.end()) {
    *val = kv_.at(key);
    return true;
  }
  return false;
}

bool Buffer::Delete(int32_t key) {
  return Put(key, TOMBSTOME);
}

}  // namespace ghostdb
