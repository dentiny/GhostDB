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

#include "common.h"
#include "run.h"

#pragma once

namespace ghostdb {

class Level {
 public:
  Level(int level);
  bool DumpTable(const std::map<Key, Val>& memtable);

 private:
  int GetAvaiRun() const;

 private:
  int level_;
  int cur_size_;
  int max_run_size_;
  std::vector<std::unique_ptr<Run>> runs_;
};

}  // namespace ghostdb
