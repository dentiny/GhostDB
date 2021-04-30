//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// sstable_page.h
//
// Identification: src/sstable_page.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cstdint>

#include "bloom.h"
#include "common.h"
#include "page.h"

namespace ghostdb {

class SSTablePage : public Page {
 public:
  SSTablePage();
  ~SSTablePage() = default;
  bool ParsePageData();
  inline int GetLevel() const { return level_; }
  inline int GetRun() const { return run_; }
  inline int32_t GetKvNum() const { return kv_num_; }
  inline Bloom GetBloomFilter() const { return bloom_filter_; }
  inline void GetMemtable(sstable_t *memtable) { memtable->swap(memtable_); }

 private:
  int level_;
  int run_;
  int32_t kv_num_;
  Bloom bloom_filter_;
  sstable_t memtable_;
};

}  // namespace ghostdb
