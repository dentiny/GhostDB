//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// page.cc
//
// Identification: src/page.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include <cassert>

#include "logger.h"
#include "page.h"

using std::bitset;
using std::pair;

namespace ghostdb {

Page::Page(char *page_data) {
  size_t offset = 0;

  // Read magic number and has_next_page.
  uint8_t header;
  memmove(&header, page_data + offset, sizeof(header));
  assert((header & 0xFE) == PAGE_HEADER_MAGIC);
  has_next_page_ = header & 0x1;
  offset += sizeof(header);
  
  // Read key-value pair counts.
  memmove(&kv_num_, page_data + offset, sizeof(kv_num_));
  offset += sizeof(kv_num_);

  // Read bloom filter.
  uint64_t filter;  // FIXME: hard-code # of bits
  memmove(&filter, page_data + offset, sizeof(filter));
  offset += sizeof(filter);
  bloom_filter_ = Bloom(filter);

  // Read key-value pairs.
  memtable_.reserve(kv_num_);
  Key key;
  Val val;
  for (int idx = 0; idx < kv_num_; ++idx) {
    memmove(&key, page_data + offset, sizeof(key));
    offset += sizeof(key);
    memmove(&val, page_data + offset, sizeof(val));
    offset += sizeof(val);
    memtable_.emplace_back(key, val);
  }
  assert(kv_num_ == static_cast<int32_t>(memtable_.size()));

  // Delete the page input.
  delete page_data;
}

void Page::GetMemtable(Bloom *bloom, memtable_t *memtable) {
  *bloom = bloom_filter_;
  // There should be only exchange of underlying array pointer, no data movement involved.
  memtable_.swap(*memtable);
}

}  // namespace ghostdb
