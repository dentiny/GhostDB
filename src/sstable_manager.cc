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

#include <cassert>

#include "config.h"
#include "sstable_manager.h"

using std::make_unique;
using std::map;
using std::pair;
using std::unique_ptr;
using std::vector;

namespace ghostdb {

SSTableManager::SSTableManager(DiskManager *disk_manager) :
  disk_manager_(disk_manager),
  levels_(vector<unique_ptr<Level>>(MAX_LEVEL_NUM)) {}

/*
 * Policy for dumping SSTable:
 * (1) search from bottom level to topmost
 * (2) search from left-most run to right-most 
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
      LOG_DEBUG("Could persist to level ", *level_no, ", run 0");
      return true;
    }
    int avai_run = levels_[idx]->GetAvaiRun();
    if (avai_run != -1) {
      *level_no = idx;
      *run_no = avai_run;
      LOG_DEBUG("Dump memtable to level ", *level_no, ", run ", *run_no);
      return true;
    }
  }
  LOG_DEBUG("No available run, compaction needed");
  return false;
}

// @return: true for dump succeeds, false for no available run, minor compaction needed
bool SSTableManager::DumpSSTable(const map<Key, Val>& memtable) {
  int level_no = -1;
  int run_no = -1;
  bool can_dump = GetAvaiRun(&level_no, &run_no);
  if (can_dump) {
    if (levels_[level_no] == nullptr) {
      levels_[level_no] = make_unique<Level>(level_no, disk_manager_);
      run_no = 0;
    }
    levels_[level_no]->DumpSSTable(run_no, memtable);
    return true;
  }
  return false;
}

void SSTableManager::DumpSSTable(int level_no, int run_no, const memtable_t& memtable) {
  assert(levels_[level_no] != nullptr);
  levels_[level_no]->DumpSSTable(run_no, memtable);
}

// Leverage Run::DumpSSTable infrastructure to decode memtable, thus use
// level=MAX_LEVEL_NUM-1, run=0, which is guarenteed to exist.
void SSTableManager::DumpTempSSTable(const memtable_t& memtable) {
  levels_[MAX_LEVEL_NUM - 1]->DumpSSTable(0 /* run */, memtable, true /* for_temp_table */);
}

void SSTableManager::LoadSSTable(int level_no, int run_no, Bloom *filter, memtable_t *memtable) {
  assert(levels_[level_no] != nullptr);
  levels_[level_no]->LoadSSTable(run_no, filter, memtable);
}

void SSTableManager::ClearTempSSTable() {
  char page_data[PAGE_SIZE] = { 0 };
  disk_manager_->WriteDb(page_data, PAGE_SIZE);
}

void SSTableManager::ClearSSTable(int level_no, int run_no) {
  assert(levels_[level_no] != nullptr);
  levels_[level_no]->ClearSSTable(run_no);
}

bool SSTableManager::IsEmpty(int level_no, int run_no) const {
  return levels_[level_no] == nullptr || levels_[level_no]->IsEmpty(run_no);
}

}  // namespace ghostdb
