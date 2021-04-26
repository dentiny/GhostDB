//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// run.cc
//
// Identification: src/run.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include <string>

#include "logger.h"
#include "run.h"
#include "string_util.h"

using std::make_unique;
using std::string;
using std::unique_ptr;

namespace ghostdb {

Run::Run(int level, int run) :
  level_(level),
  run_(run),
  is_empty_(true),
  filter_(make_unique<Bloom>()),
  disk_manager_(make_unique<DiskManager>(level, run)) {}

bool Run::DumpTable(const std::map<Key, Val>& memtable) {
  is_empty_ = false;
  filter_->ClearKey();
  for (auto& kv : memtable) {
    filter_->SetKey(kv.first);
  }
  string filter_keys = filter_->ToString();
  disk_manager_->AppendDb(filter_keys.c_str(), static_cast<int>(filter_keys.size()));
  string kv = MemtableToString(memtable);
  disk_manager_->AppendDb(kv.c_str(), static_cast<int>(kv.size()));
  disk_manager_->FlushDb();
  return true;
}

}  // namespace ghostdb
