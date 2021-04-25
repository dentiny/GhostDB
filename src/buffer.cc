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

#include <cassert>
#include <string>

#include "buffer.h"

using std::make_unique;

namespace ghostdb {

Buffer::Buffer(uint32_t buffer_size) :
  cur_size_(0),
  buffer_size_(buffer_size),
  log_manager_(make_unique<LogManager>()) {}

/*
 * @return: true for place succeeds in buffer, false for buffer full
 */
bool Buffer::Put(int32_t key, int32_t val) {
  if (cur_size_ == buffer_size_ && kv_.find(key) == kv_.end()) {
    LOG_DEBUG("Buffer has been full.");
    return false;
  }
  ++cur_size_;
  kv_[key] = val;
  log_manager_->AppendLogRecord(key, val);
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
