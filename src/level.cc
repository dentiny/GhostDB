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

#include "disk_manager.h"
#include "level.h"

using std::make_unique;
using std::map;
using std::string;
using std::unique_ptr;
using std::vector;

namespace ghostdb {

Level::Level(int level) :
  level_(level),
  cur_size_(0),
  max_run_size_((level + 1) * MAX_RUN_PER_LEVEL),
  runs_(vector<unique_ptr<Run>>(max_run_size_)) {}

int Level::GetAvaiRun() const {
  for (int idx = 0; idx < max_run_size_; ++idx) {
    if (runs_[idx] == nullptr || runs_[idx]->IsEmpty()) {
      return idx;
    }
  }
  return -1;
}

bool Level::DumpTable(int run_no, const map<Key, Val>& memtable) {
  assert(runs_[run_no] == nullptr || runs_[run_no]->IsEmpty());
  if (runs_[run_no] == nullptr) {
    runs_[run_no] = make_unique<Run>(level_, run_no);
  }
  runs_[run_no]->DumpTable(memtable);
  return true;
}

void Level::LoadTable(int run_no, Bloom *filter, memtable_t *memtable) {
  assert(runs_[run_no] != nullptr && !runs_[run_no]->IsEmpty());
  runs_[run_no]->LoadTable(filter, memtable);
}

void Level::RemoveTable(int run_no) {
  assert(runs_[run_no] != nullptr && !runs_[run_no]->IsEmpty());
  runs_[run_no]->RemoveTable();
}

void Level::MergeSSTableTo(int run_no) {
  assert(runs_[run_no] != nullptr && runs_[run_no]->IsEmpty());
  runs_[run_no]->MergeSSTableTo();
}

bool Level::IsEmpty(int run_no) const {
  return runs_[run_no] == nullptr || runs_[run_no]->IsEmpty();
}

}  // namespace ghostdb
