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
  void GetMemtable(Bloom *filter, memtable_t *memtable);

 private:
  int32_t kv_num_;
  Bloom bloom_filter_;
  memtable_t memtable_;
};

}  // namespace ghostdb
