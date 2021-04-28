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

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include "logger.h"
#include "page.h"
#include "run.h"
#include "util.h"

using std::make_unique;
using std::map;
using std::pair;
using std::string;
using std::unique_ptr;
using std::vector;

namespace ghostdb {

Run::Run(int level, int run) :
  level_(level),
  run_(run),
  is_empty_(true),
  disk_manager_(make_unique<DiskManager>(level, run)) {
  assert((level == TEMP_LEVEL_NO && run == TEMP_RUN_NO) ||
         (level < MAX_LEVEL_NUM && run < (level + 1) * MAX_RUN_PER_LEVEL));
}

// About page layout, reference to: page.h
// TODO: currently don't consider the situation memtable demands more than one page, set has_next_page = 0
template<typename Cont>
bool Run::DumpTable(const Cont& memtable) {
  is_empty_ = false;
  size_t offset = 0;
  char page_data[PAGE_SIZE] = { 0 };

  // Dump magic number(and has_next_page bit).
  memmove(page_data + offset, &PAGE_HEADER_MAGIC, sizeof(PAGE_HEADER_MAGIC));
  offset += sizeof(PAGE_HEADER_MAGIC);

  // Dump # of key-value pairs.
  kv_num_ = static_cast<int32_t>(memtable.size());
  memmove(page_data + offset, &kv_num_, sizeof(kv_num_));
  offset += sizeof(kv_num_);

  // Dump bloom filter.
  // NOTE: Dump and load in the same format.
  // Specifically, bloom filter lives as 64-bit integers in files(cannot be dumped in the string format).
  bloom_filter_.ClearKey();
  for (auto& kv : memtable) {
    bloom_filter_.SetKey(kv.first);
  }
  uint64_t val_to_dump = bloom_filter_.ToInt();
  memmove(page_data + offset, &val_to_dump, sizeof(val_to_dump));
  offset += sizeof(val_to_dump);

  // Dump key-value pairs.
  string kv = MemtableToString(memtable);
  memmove(page_data + offset, kv.c_str(), kv.size());
  disk_manager_->WriteDb(page_data, PAGE_SIZE);
  return true;
}

// Explicit instantiation to export symbol.
template bool Run::DumpTable(const vector<pair<Key, Val>>& memtable);
template bool Run::DumpTable(const map<Key, Val>& memtable);

void Run::LoadTable(Bloom *filter, memtable_t *memtable) {
  disk_manager_->ReadDb(filter, memtable);
  assert(bloom_filter_ == *filter);
}

void Run::RemoveTable() {
  is_empty_ = true;
  disk_manager_->RemoveTable();
}

}  // namespace ghostdb
