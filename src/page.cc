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
#include <cstring>

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

  LOG_DEBUG("header & 0xFE = ", header & 0xFE);
  LOG_DEBUG("PAGE_HEADER_MAGIC = ", PAGE_HEADER_MAGIC);

  assert((header & 0xFE) == PAGE_HEADER_MAGIC);
  has_next_page_ = header & 0x1;
  offset += sizeof(header);
  
  // Read key-value pair counts.
  memmove(&kv_num_, page_data + offset, sizeof(kv_num_));
  offset += sizeof(kv_num_);

  // Read bloom filter.
  int8_t filter;  // FIXME: hard-code # of bits
  memmove(&filter, page_data + offset, sizeof(filter));
  offset += sizeof(filter);
  bloom_filter_ = bitset<BLOOM_BITS / 8>(filter);

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

  // Delete the page input.
  delete page_data;
}

void Page::GetMemtable(memtable_t *memtable) {
  // There should be only exchange of underlying array pointer, no data movement involved.
  memtable_.swap(*memtable);
}

}  // namespace ghostdb
