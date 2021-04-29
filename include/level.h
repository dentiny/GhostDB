//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// level.h
//
// Identification: src/level.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

/*
 * Persistence policy:
 * (1) start from the upper level(level = 0)
 * (2) start from rightmost(with max run id)
 * Compaction policy:
 * (1) start from the lower level(with max level id)
 * (2) compact from the leftmost(run = 0)
 */

#include <map>
#include <utility>
#include <vector>

#include "bloom.h"
#include "common.h"
#include "disk_manager.h"
#include "run.h"

#pragma once

namespace ghostdb {

class Level {
 public:
  Level(int level, DiskManager *disk_manager);
  int GetAvaiRun() const;
  void ClearSSTable(int run_no);
  template<typename Cont>
  bool DumpSSTable(int run_no, const Cont& memtable);
  void LoadSSTable(int run_no, Bloom *filter, memtable_t *memtable);
  bool IsEmpty(int run_no) const;

 private:
  int level_;
  int cur_size_;
  int max_run_size_;
  DiskManager *disk_manager_;
  std::vector<std::unique_ptr<Run>> runs_;
};

}  // namespace ghostdb
