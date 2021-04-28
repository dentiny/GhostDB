//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// compaction_manager.cc
//
// Identification: src/compaction_manager.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include <map>
#include <string>

#include "bloom.h"
#include "compaction_manager.h"
#include "config.h"
#include "logger.h"
#include "run.h"
#include "util.h"

using std::make_unique;
using std::map;
using std::string;

namespace ghostdb {

CompactionManager::CompactionManager(SSTableManager *sstable_manager) :
  enable_compaction_(true),
  sstable_manager_(sstable_manager) {
  RunCompactionThread();
}

CompactionManager::~CompactionManager() {
  StopCompactionThread();
}

void CompactionManager::RunCompactionThread() {
  LOG_DEBUG("Start compaction thread");
  compaction_future_ = std::async([&]() {
    while (enable_compaction_) {
      std::unique_lock<std::mutex> lck(latch_);
      compaction_cv_.wait_for(lck, COMPACTION_TIMEOUT, [&]{
        return request_minor_compaction_.load() || request_major_compaction_.load();
      });
      // major compaction owns precedence over minor compaction
      if (request_major_compaction_) {
        LaunchMajorCompaction();
        request_major_compaction_ = false;
        request_minor_compaction_ = false;
      } else if (request_minor_compaction_) {
        LaunchMinorCompaction();
        request_minor_compaction_ = false;
      }
      wait_compaction_cv_.notify_all();
    }
  });
}

void CompactionManager::StopCompactionThread() {
  RequestMajorCompaction();
  enable_compaction_ = false;
  compaction_future_.get();
}

void CompactionManager::RequestMinorCompaction() {
  std::unique_lock<std::mutex> lck(latch_);
  request_minor_compaction_ = true;
  compaction_cv_.notify_one();
  wait_compaction_cv_.wait(lck, [&]() { return !request_minor_compaction_; });  // block until compaction completes
}

void CompactionManager::RequestMajorCompaction() {
  std::unique_lock<std::mutex> lck(latch_);
  request_major_compaction_ = true;
  compaction_cv_.notify_one();
  wait_compaction_cv_.wait(lck, [&]() { return !request_major_compaction_; });  // block until compaction completes
}

// Invoked with latch_ held.
// TODO: currently doesn't dump intermediary memtable into temporary files
void CompactionManager::LaunchMinorCompaction() {
  LOG_DEBUG("Launch minor compaction");
  LaunchCompactionImpl(MINOR_COMPACTION_LEVEL_NUM - 1 /* start_level */);
}

// Invoked with latch_ held.
void CompactionManager::LaunchMajorCompaction() {
  LOG_DEBUG("Launch major compaction");
  LaunchCompactionImpl(MAX_LEVEL_NUM - 1 /* start_level */);
}

void CompactionManager::LaunchCompactionImpl(int start_level_no) {
  bool is_run_avai = true;  // to get out of the double loop

  // Load latest SSTables, and compact them in the memory.
  memtable_t memtable;
  for (int level_no = start_level_no; is_run_avai && level_no >= 0; --level_no) {
    for (int run_no = 0; run_no < (level_no + 1) * MAX_RUN_PER_LEVEL; ++run_no) {
      if (sstable_manager_->IsEmpty(level_no, run_no)) {
        is_run_avai = false;
      }
      LOG_DEBUG("During compaction, try to load level:", level_no, ", run:", run_no);
      Bloom bloom_filter;
      memtable_t sstable_kv;
      sstable_manager_->LoadTable(level_no, run_no, &bloom_filter, &sstable_kv);
      memtable = MergeSSTable(sstable_kv /* new */, memtable /* old */);
    }
  }

  // Write to temporary files first in case of fs failure, and switch back to SSTable files.
  is_run_avai = true;
  Run temp_run(TEMP_LEVEL_NO, TEMP_RUN_NO);
  temp_run.DumpTable(memtable);
  for (int level_no = start_level_no; is_run_avai && level_no >= 0; --level_no) {
    for (int run_no = 0; run_no < (level_no + 1) * MAX_RUN_PER_LEVEL; ++run_no) {
      if (sstable_manager_->IsEmpty(level_no, run_no)) {
        is_run_avai = false;
        break;
      }
      LOG_DEBUG("During compaction, try to remove level:", level_no, ", run:", run_no);
      sstable_manager_->RemoveTable(level_no, run_no);
    }
  }
  sstable_manager_->MergeSSTableTo(start_level_no /* level */, 0 /* run */);
  LOG_DEBUG("Merge SSTables to level:", start_level_no, ", run:", 0);
}

}  // namespace ghostdb
