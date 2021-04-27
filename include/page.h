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
 * | magic number(7 bit) | has_next_page(1 bit) | # of key-value pairs (32 bit) |
 * | bloom filter(BLOOM_BITS, defined at config.h) | memtable |
 */

#pragma once

#include <bitset>
#include <cstdint>
#include <utility>
#include <vector>

#include "common.h"
#include "config.h"

namespace ghostdb {

class Page {
 public:
  Page(char *page_data);
  ~Page() = default;
  inline bool HasNextPage() const { return has_next_page_; }
  void GetMemtable(std::vector<std::pair<Key, Val>> *memtable);

 public:
  static constexpr int PAGE_HEADER_SIZE = 8;  // magic number and has_next_page bit
  static constexpr int KV_PAIR_NUM_IN_PAGE_HEADER = 4;

 private:
  bool has_next_page_;
  int32_t kv_num_;
  std::bitset<BLOOM_BITS / 8> bloom_filter_;
  std::vector<std::pair<Key, Val>> memtable_;
};

}  // namespace ghostdb
