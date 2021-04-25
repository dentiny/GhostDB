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

#include <map>
#include <memory>

#include "bloom.h"
#include "disk_manager.h"

namespace ghostdb {

class Run {
 public:
  Run(int level, int run);
  bool IsEmpty() const { return is_empty_; }
  bool DumpTable(const std::map<Key, Val>& memtable);

 private:
  int level_;
  int run_;
  bool is_empty_;  // whether there's corresponding SSTable
  std::unique_ptr<Bloom> filter_;
  std::unique_ptr<DiskManager> disk_manager_;
};

}  // namespace ghostdb
