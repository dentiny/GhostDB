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
  for (int idx = max_run_size_ - 1; idx >= 0; --idx) {
    if (runs_[idx] == nullptr || runs_[idx]->IsEmpty()) {
      return idx;
    }
  }
  return -1;
}

/*
 * @return: true for dump memtable in first level, false for first level has 
 */
bool Level::DumpTable(const std::map<Key, Val>& memtable) {
  assert(level_ == 0);  // DumpTable only invoked in GhostDB::Put
  int avai_run = GetAvaiRun();
  if (avai_run == -1) {
    return false;
  }
  if (runs_[avai_run] == nullptr) {
    runs_[avai_run] = make_unique<Run>(level_, avai_run);
  }
  runs_[avai_run]->DumpTable(memtable);
  return true;
}

}  // namespace ghostdb
