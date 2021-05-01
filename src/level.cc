//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// level.cc
//
// Identification: src/level.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include <cassert>
#include <memory>
#include <string>
#include <type_traits>

#include "level.h"

using std::is_same_v;
using std::make_unique;
using std::map;
using std::pair;
using std::string;
using std::unique_ptr;
using std::vector;

namespace ghostdb {

Level::Level(int level, DiskManager *disk_manager) :
  level_(level),
  cur_size_(0),
  max_run_size_((level + 1) * MAX_RUN_PER_LEVEL),
  disk_manager_(disk_manager),
  runs_(vector<unique_ptr<Run>>(max_run_size_)) {
  for (int run_no = 0; run_no < max_run_size_; ++run_no) {
    runs_[run_no] = make_unique<Run>(level_, run_no, disk_manager_);
  }
}

int Level::GetAvaiRun() const {
  for (int idx = 0; idx < max_run_size_; ++idx) {
    if (runs_[idx]->IsEmpty()) {
      return idx;
    }
  }
  return -1;
}

void Level::ClearSSTable(int run_no) {
  assert(!runs_[run_no]->IsEmpty());
  runs_[run_no]->ClearSSTable();
}

template<typename Cont>
bool Level::DumpSSTable(int run_no, const Cont& memtable, bool for_temp_table) {  
  // When dumping temporary compaction SSTable files(whether minor or major), there's
  // no guarentee whether the run is empty.
  if constexpr (is_same_v<buffer_t, Cont>) {
    assert(runs_[run_no]->IsEmpty());
  }

  runs_[run_no]->DumpSSTable(memtable, for_temp_table);
  return true;
}

// Explicit instantiation to export symbol.
template bool Level::DumpSSTable(int run_no, const sstable_t& memtable, bool for_temp_table);
template bool Level::DumpSSTable(int run_no, const buffer_t& memtable, bool for_temp_table);

// @return: true for SSTable file exists
bool Level::LoadSSTable(int run_no, Bloom *filter, sstable_t *memtable) {
  if (runs_[run_no]->IsEmpty()) {
    return false;
  }
  runs_[run_no]->LoadSSTable(filter, memtable);
  return true;
}

bool Level::IsEmpty(int run_no) const {
  return runs_[run_no]->IsEmpty();
}

}  // namespace ghostdb
