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
#include <vector>

#include "bloom.h"
#include "common.h"
#include "run.h"

#pragma once

namespace ghostdb {

class Level {
 public:
  Level(int level);
  int GetAvaiRun() const;
  bool DumpTable(int run_no, const std::map<Key, Val>& memtable);
  void LoadTable(int run_no, Bloom *filter, memtable_t *memtable);
  void RemoveTable(int run_no);
  void MergeSSTableTo(int run_no);
  bool IsEmpty(int run_no) const;

 private:
  int level_;
  int cur_size_;
  int max_run_size_;
  std::vector<std::unique_ptr<Run>> runs_;
};

}  // namespace ghostdb
