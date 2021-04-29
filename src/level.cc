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

#include "level.h"

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
  runs_(vector<unique_ptr<Run>>(max_run_size_)) {}

int Level::GetAvaiRun() const {
  for (int idx = 0; idx < max_run_size_; ++idx) {
    if (runs_[idx] == nullptr || runs_[idx]->IsEmpty()) {
      return idx;
    }
  }
  return -1;
}

void Level::ClearSSTable(int run_no) {
  assert(runs_[run_no] != nullptr && !runs_[run_no]->IsEmpty());
  runs_[run_no]->ClearSSTable();
}

template<typename Cont>
bool Level::DumpSSTable(int run_no, const Cont& memtable) {
  assert(runs_[run_no] == nullptr || runs_[run_no]->IsEmpty());
  if (runs_[run_no] == nullptr) {
    runs_[run_no] = make_unique<Run>(level_, run_no, disk_manager_);
  }
  runs_[run_no]->DumpSSTable(memtable);
  return true;
}

// Explicit instantiation to export symbol.
template bool Level::DumpSSTable(int run_no, const vector<pair<Key, Val>>& memtable);
template bool Level::DumpSSTable(int run_no, const map<Key, Val>& memtable);

void Level::LoadSSTable(int run_no, Bloom *filter, memtable_t *memtable) {
  assert(runs_[run_no] != nullptr && !runs_[run_no]->IsEmpty());
  runs_[run_no]->LoadSSTable(filter, memtable);
}

bool Level::IsEmpty(int run_no) const {
  return runs_[run_no] == nullptr || runs_[run_no]->IsEmpty();
}

}  // namespace ghostdb
