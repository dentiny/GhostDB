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

#include <cstring>
#include <string>

#include "logger.h"
#include "page.h"
#include "run.h"
#include "util.h"

using std::make_unique;
using std::map;
using std::string;
using std::unique_ptr;

namespace ghostdb {

Run::Run(int level, int run) :
  level_(level),
  run_(run),
  is_empty_(true),
  filter_(make_unique<Bloom>()),
  disk_manager_(make_unique<DiskManager>(level, run)) {}

// About page layout, reference to: page.h
// TODO: currently don't consider the situation memtable demands more than one page, set has_next_page = 0
bool Run::DumpTable(const map<Key, Val>& memtable) {
  is_empty_ = false;
  size_t offset = 0;
  char page_data[PAGE_SIZE] = { 0 };

  // Dump magic number.
  memmove(page_data + offset, &PAGE_HEADER_MAGIC, sizeof(PAGE_HEADER_MAGIC));
  offset += sizeof(PAGE_HEADER_MAGIC);

  // Dump # of key-value pairs.
  int32_t kv_num = static_cast<int32_t>(memtable.size());
  memmove(page_data + offset, &kv_num, sizeof(kv_num));
  offset += sizeof(kv_num);

  // Dump bloom filter.
  filter_->ClearKey();
  for (auto& kv : memtable) {
    filter_->SetKey(kv.first);
  }
  string filter_keys = filter_->ToString();
  memmove(page_data + offset, filter_keys.c_str(), filter_keys.size());
  offset += filter_keys.size();

  // Dump key-value pairs.
  string kv = MemtableToString(memtable);
  memmove(page_data + offset, kv.c_str(), kv.size());
  disk_manager_->WriteDb(page_data, PAGE_SIZE);
  return true;
}

void Run::LoadTable(memtable_t *memtable) {
  disk_manager_->LoadTable(memtable);
}

}  // namespace ghostdb
