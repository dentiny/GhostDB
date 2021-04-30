//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// page.h
//
// Identification: src/page.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

/*
 * Page Layout
 * | magic number(8 bit) | # of key-value pairs (32 bit) | page_id (32 bit) |
 * | bloom filter(BLOOM_BITS, defined at config.h) | memtable |
 */

#pragma once

#include <bitset>
#include <cstdint>
#include <cstring>
#include <utility>
#include <vector>

#include "bloom.h"
#include "common.h"
#include "config.h"

namespace ghostdb {

class Page {
  friend class RecoveryManager;
  friend class Run;

  // Check page layout
  static_assert(sizeof(PAGE_HEADER_MAGIC) == 1);

 public:
  Page() { memset(data_, 0, PAGE_SIZE); }
  ~Page() = default;
  inline int32_t GetPageId() const { return page_id_; }

 public:
  static constexpr int PAGE_HEADER_SIZE = 1;  // magic number
  static constexpr int KV_PAIR_NUM_IN_PAGE_HEADER = 4;
  static constexpr int PAGE_ID = 4;
  static constexpr int PAYLOAD_OFFSET =
                        PAGE_HEADER_SIZE + KV_PAIR_NUM_IN_PAGE_HEADER +
                        PAGE_ID + BLOOM_BITS / 8;

 protected:
  /** page id within SSTable file */
  int32_t page_id_;
  /** The actual data that is stored within a page. */
  char data_[PAGE_SIZE]{};
};

}  // namespace ghostdb
