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
  Run(int level, int run, DiskManager *disk_manager);
  bool IsEmpty() const { return is_empty_; }
  void ClearSSTable();
  template<typename Cont>
  bool DumpSSTable(const Cont& memtable, bool for_temp_table);
  void LoadSSTable(Bloom *filter, memtable_t *memtable);

 private:
  int level_;
  int run_;
  bool is_empty_;  // whether there's corresponding SSTable
  int32_t kv_num_;  // # of key-value pairs in the SSTable
  Bloom bloom_filter_;
  DiskManager *disk_manager_;
};

}  // namespace ghostdb
