//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// clock_replacer.cc
//
// Identification: src/clock_replacer.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include <cassert>

#include "clock_replacer.h"

using std::make_pair;
using std::mutex;
using std::pair;
using std::unique_lock;
using std::vector;

namespace ghostdb {

ClockReplacer::ClockReplacer(int num_sstable) :
  size_(0),
  capacity_(num_sstable),
  hand_(0),
  refs_(vector<pair<bool, bool>>(capacity_, make_pair(false, false))) {}

ClockReplacer::~ClockReplacer() = default;

bool ClockReplacer::Victim(sstable_id_t *sstable_id) {
  unique_lock<mutex> lck(mtx_);
  if (size_ == 0) {
    return false;
  }

  while (true) {
    assert(hand_ < capacity_);
    auto &entry = refs_[hand_];
    if (entry.first) {     // exists
      if (entry.second) {  // referenced
        entry.second = false;
      } else {  // not referenced
        *sstable_id = static_cast<sstable_id_t>(hand_);
        entry.first = false;
        --size_;
        break;
      }
    }
    hand_ = (hand_ + 1) % capacity_;
  }
  return true;
}

void ClockReplacer::Pin(sstable_id_t sstable_id) {
  unique_lock<mutex> lck(mtx_);
  assert(static_cast<int>(sstable_id) < capacity_);
  auto &entry = refs_[sstable_id];
  if (entry.first) {
    entry.first = false;
    entry.second = false;
    --size_;
  }
}

void ClockReplacer::Unpin(sstable_id_t sstable_id) {
  unique_lock<mutex> lck(mtx_);
  assert(static_cast<int>(sstable_id) < capacity_);
  auto &entry = refs_[sstable_id];
  if (!entry.first) {
    entry.first = true;
    entry.second = true;
    ++size_;
  }
}

int ClockReplacer::Size() {
  unique_lock<mutex> lck(mtx_);
  return size_;
}

}  // namespace ghostdb
