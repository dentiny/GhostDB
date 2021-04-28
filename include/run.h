//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// run.h
//
// Identification: src/run.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "bloom.h"
#include "common.h"
#include "disk_manager.h"
#include "page.h"

namespace ghostdb {

class Run {
 public:
  Run(int level, int run);
  bool IsEmpty() const { return is_empty_; }
  template<typename Cont>
  bool DumpTable(const Cont& memtable);
  void LoadTable(Bloom *filter, memtable_t *memtable);
  void RemoveTable();

 private:
  int level_;
  int run_;
  bool is_empty_;  // whether there's corresponding SSTable
  int32_t kv_num_;  // # of key-value pairs in the SSTable
  Bloom bloom_filter_;
  std::unique_ptr<DiskManager> disk_manager_;
};

}  // namespace ghostdb
