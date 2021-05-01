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

Buffer::Buffer(uint32_t buffer_size, DiskManager *disk_manager) :
  buffer_size_(buffer_size),
  log_manager_(make_unique<LogManager>(disk_manager)) {}

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

void Buffer::GetRange(Key key1, Key key2, buffer_t *res) {
  assert(key1 <= key2);
  const auto& kv1 = *kv_.begin();
  const auto& kv2 = *kv_.rbegin();
  if (kv1.first > key2 || kv2.first < key1) {
    return;
  }
  auto it1 = kv_.lower_bound(key1);
  auto it2 = kv_.lower_bound(key2);
  if (it2 != kv_.end()) {
    ++it2;
  }
  for (auto it = it1; it != it2; ++it) {
    Key key = it->first;
    Val val = it->second;
    (*res)[key] = val;
  }
}

}  // namespace ghostdb
