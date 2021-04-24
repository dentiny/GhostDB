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

#include <string>

#include "buffer.h"

namespace ghostdb {

Buffer::Buffer(uint32_t buffer_size) :
  buffer_size_(buffer_size),
  disk_manager_(std::make_unique<DiskManager>()) {}

bool Buffer::Put(int32_t key, int32_t val) {
  if (kv_.size() == buffer_size_ && kv_.find(key) == kv_.end()) {
    LOG_DEBUG("Buffer has been full.");
    return false;
  }
  kv_[key] = val;
  return true;
}

bool Buffer::Get(int32_t key, int32_t *val) {
  if (kv_.find(key) != kv_.end() && kv_.at(key) != TOMBSTOME) {
    *val = kv_.at(key);
    return true;
  }
  return false;
}

bool Buffer::Delete(int32_t key) {
  return Put(key, TOMBSTOME);
}

}  // namespace ghostdb
