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
  buffer_size_(buffer_size),
  log_manager_(make_unique<LogManager>()) {}

/*
 * @return: true for place succeeds in buffer, false for buffer full
 */
bool Buffer::Put(Key key, Val val) {
  if (kv_.size() == buffer_size_ && kv_.find(key) == kv_.end()) {
    LOG_DEBUG("Buffer has been full.");
    return false;
  }
  kv_[key] = val;
  log_manager_->AppendLogRecord(key, val);
  return true;
}

/*
 * @param[out]: only meaningful when get value successful, set to nullptr if deleted 
 * @return: true for get value succeeds, false for not in buffer
 */
bool Buffer::Get(Key key, Val **val) {
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
