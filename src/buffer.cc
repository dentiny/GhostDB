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

/*
 * @return: true for place succeeds in buffer, false for buffer full
 */
bool Buffer::Put(int32_t key, int32_t val) {
  if (kv_.size() == buffer_size_ && kv_.find(key) == kv_.end()) {
    LOG_DEBUG("Buffer has been full.");
    return false;
  }
  kv_[key] = val;
  return true;
}

/*
 * @param[out]: only meaningful when get value successful, set to nullptr if deleted 
 * @return: true for get value succeeds, false for not in buffer
 */
bool Buffer::Get(int32_t key, int32_t **val) {
  if (kv_.find(key) != kv_.end()) {
    if (kv_.at(key) == TOMBSTOME) {
      *val = nullptr;
    } else {
      **val = kv_.at(key);
    }
    return true;
  }
  return false;
}

}  // namespace ghostdb
