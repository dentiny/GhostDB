//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// sstable_manager.cc
//
// Identification: src/sstable_manager.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include "config.h"
#include "sstable_manager.h"

using std::make_unique;
using std::unique_ptr;
using std::vector;

namespace ghostdb {

SSTableManager::SSTableManager() :
  levels_(vector<unique_ptr<Level>>(MAX_LEVEL_NUM)) {}

/*
 * Merge down policy:
 * (1) Search from bottom to top
 * (2) Search from left to right
 *
 * @param[out] level_no: only meaningful when return true
 * @param[out] run_no: only meaningful when return true, -1 if level not allocated yet
 * @return: whether there's available run
 */
bool SSTableManager::GetAvaiRun(int *level_no, int *run_no) const {
  for (int idx = MAX_LEVEL_NUM - 1; idx >= 0; --idx) {
    if (levels_[idx] == nullptr) {
      *level_no = idx;
      *run_no = -1;
      LOG_DEBUG("Could persist to empty level ", *level_no);
      return true;
    }
    int avai_run = levels_[idx]->GetAvaiRun();
    if (avai_run != -1) {
      *level_no = idx;
      *run_no = avai_run;
      LOG_DEBUG("Could persist to level ", *level_no, ", run ", *run_no);
      return true;
    }
  }
  LOG_DEBUG("No available run, compaction needed");
  return false;
}

// @return: true for dump succeeds, false for no available run, minor compaction needed
bool SSTableManager::DumpTable(const std::map<Key, Val>& memtable) {
  int level_no = -1;
  int run_no = -1;
  bool can_dump = GetAvaiRun(&level_no, &run_no);
  if (can_dump) {
    if (levels_[level_no] == nullptr) {
      levels_[level_no] = make_unique<Level>(level_no);
      run_no = 0;
    }
    levels_[level_no]->DumpTable(run_no, memtable);
    return true;
  }
  return false;
}

}  // namespace ghostdb
