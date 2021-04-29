//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// sstable_page.cc
//
// Identification: src/sstable_page.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include <cassert>

#include "sstable_page.h"

namespace ghostdb {

SSTablePage::SSTablePage() :
  kv_num_(-1) {}

void SSTablePage::GetMemtable(Bloom *bloom_filter, memtable_t *memtable) {
  size_t offset = 0;

  // Read magic number and has_next_page.
  uint8_t header;
  memmove(&header, data_ + offset, sizeof(header));
  assert(header == PAGE_HEADER_MAGIC);
  offset += sizeof(header);
  
  // Read key-value pair counts.
  memmove(&kv_num_, data_ + offset, sizeof(kv_num_));
  offset += sizeof(kv_num_);

  // Read bloom filter.
  uint64_t filter;  // FIXME: hard-code # of bits
  memmove(&filter, data_ + offset, sizeof(filter));
  offset += sizeof(filter);
  bloom_filter_ = Bloom(filter);

  // Read key-value pairs.
  memtable_.reserve(kv_num_);
  Key key;
  Val val;
  for (int idx = 0; idx < kv_num_; ++idx) {
    memmove(&key, data_ + offset, sizeof(key));
    offset += sizeof(key);
    memmove(&val, data_ + offset, sizeof(val));
    offset += sizeof(val);
    memtable_.emplace_back(key, val);
  }
  assert(kv_num_ == static_cast<int32_t>(memtable_.size()));

  *bloom_filter = bloom_filter_;
  memtable->swap(memtable_);
}

}  // namespace ghostdb
