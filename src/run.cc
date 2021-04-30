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
#include <type_traits>
#include <utility>
#include <vector>

#include "logger.h"
#include "run.h"
#include "sstable_page.h"
#include "util.h"

using std::is_same_v;
using std::make_unique;
using std::map;
using std::pair;
using std::string;
using std::unique_ptr;
using std::vector;

namespace ghostdb {

Run::Run(int level, int run, DiskManager *disk_manager) :
  level_(level),
  run_(run),
  is_empty_(true),
  disk_manager_(disk_manager) {
  assert(level < MAX_LEVEL_NUM && run < GetMaxRun(level));
  SSTablePage sstable_page;
  disk_manager_->ReadDb(sstable_page.data_, GetPageId(level_, run_));
  is_empty_ = sstable_page.ParsePageData();
  LOG_DEBUG("Run(", level_, ", ", run_, ")", is_empty_ ? " is empty" : "is not empty");
}

void Run::ClearSSTable() {
  assert(!is_empty_);
  is_empty_ = true;
  disk_manager_->WriteDb(zero_data, PAGE_SIZE, GetPageId(level_, run_));
}

// About page layout, reference to: page.h
// TODO: currently don't consider the situation memtable demands more than one page, set has_next_page = 0
template<typename Cont>
bool Run::DumpSSTable(const Cont& memtable, bool for_temp_table) {
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

  // Dump page id.
  int32_t page_id = GetPageId(level_, run_);
  memmove(page_data + offset, &page_id, sizeof(page_id));
  offset += sizeof(page_id);

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
  // TODO: total storage space needed is related to MAX_BUFFER_SIZE(cannot exceed PAGE_SIZE), currently doesn't support next_page_id.
  string kv = MemtableToString(memtable);
  memmove(page_data + offset, kv.c_str(), kv.size());

  // For temporary SSTable files before dumping into the SSTable file.
  if (for_temp_table) {
    disk_manager_->WriteDb(page_data, PAGE_SIZE);
  } else {
    disk_manager_->WriteDb(page_data, PAGE_SIZE, GetPageId(level_, run_));
  }
  return true;
}

// Explicit instantiation to export symbol.
template bool Run::DumpSSTable(const sstable_t& memtable, bool for_temp_table);
template bool Run::DumpSSTable(const buffer_t& memtable, bool for_temp_table);

void Run::LoadSSTable(Bloom *filter, sstable_t *memtable) {
  assert(!is_empty_);
  SSTablePage sstable_page;
  disk_manager_->ReadDb(sstable_page.data_, GetPageId(level_, run_));
  sstable_page.ParsePageData();
  *filter = sstable_page.GetBloomFilter();
  sstable_page.GetMemtable(memtable);
}

}  // namespace ghostdb
