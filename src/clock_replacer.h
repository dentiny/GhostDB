//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// clock_replacer.h
//
// Identification: src/clock_replacer.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <mutex>
#include <utility>
#include <vector>

#include "common.h"

namespace ghostdb {

class ClockReplacer {
 public:
  /**
   * Create a new ClockReplacer.
   * @param num_pages the maximum number of pages the ClockReplacer will be required to store
   */
  explicit ClockReplacer(int num_sstable);

  /**
   * Destroys the ClockReplacer.
   */
  ~ClockReplacer();

  bool Victim(sstable_id_t *sstable_id);

  // Called after a page is pinned to a frame in BufferPoolManager; remove the
  // frame containing the pinned page from ClockReplacer.
  void Pin(sstable_id_t sstable_id);

  // Called when the pin_count of a page becomes 0; add the frame containing
  // the unpinned page to the ClockReplacer.
  void Unpin(sstable_id_t sstable_id);

  int Size();

 private:
  int size_;  // actual size of buffer frames
  const int capacity_;
  int hand_;
  std::mutex mtx_;
  std::vector<std::pair<bool, bool>> refs_;  // <if exists, if referenced>
};

}  // namespace ghostdb
