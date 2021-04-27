//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// sstable_manager.h
//
// Identification: src/sstable_manager.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <map>
#include <vector>

#include "common.h"
#include "level.h"
#include "run.h"

namespace ghostdb {

class SSTableManager {
 public:
  SSTableManager();
  ~SSTableManager() = default;
  bool DumpTable(const std::map<Key, Val>& memtable);
  void LoadTable(int level_no, int run_no, memtable_t *memtable);

 private:
  bool GetAvaiRun(int *level_no, int *run_no) const;

 private:
  /** DB holds at most MAX_LEVEL_NUM levels, each has upper limit of runs */
  std::vector<std::unique_ptr<Level>> levels_;
};

}  // namespace ghostdb
