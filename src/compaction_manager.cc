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

#include <cassert>
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

/*
 * 1. Load SSTable pages and compact them in the memory
 * 2. Dump the compacted SSTable pages into temporary SSTable file
 * 3. Remove related pages within SSTable pages
 * 4. Copy compacted SSTables pages into the desinated page
 * 5. Remove the temporary SSTable file
 */
void CompactionManager::LaunchCompactionImpl(int start_level_no) {
  bool is_run_avai = true;  // to get out of the double loop

  // 1. Load SSTable pages and compact them in the memory
  memtable_t memtable;
  for (int level_no = start_level_no; is_run_avai && level_no >= 0; --level_no) {
    for (int run_no = 0; run_no < (level_no + 1) * MAX_RUN_PER_LEVEL; ++run_no) {
      if (sstable_manager_->IsEmpty(level_no, run_no)) {
        is_run_avai = false;
        break;
      }
      LOG_DEBUG("During compaction, load SSTable file level:", level_no, ", run:", run_no);
      Bloom bloom_filter;
      memtable_t sstable_kv;
      sstable_manager_->LoadSSTable(level_no, run_no, &bloom_filter, &sstable_kv);
      memtable = MergeSSTable(sstable_kv /* new */, memtable /* old */);
    }
  }

  // Check whether there's SSTable to compact
  if (memtable.empty()) {
    LOG_DEBUG("there's no SSTable to compact");
    return;
  }

  // 2. Dump the compacted SSTable pages into temporary SSTable file
  LOG_DEBUG("During compaction, dump temporary SSTable file");
  sstable_manager_->DumpTempSSTable(memtable);

  // 3. Remove related pages within SSTable pages
  is_run_avai = true;
  for (int level_no = start_level_no; is_run_avai && level_no >= 0; --level_no) {
    for (int run_no = 0; run_no < (level_no + 1) * MAX_RUN_PER_LEVEL; ++run_no) {
      if (sstable_manager_->IsEmpty(level_no, run_no)) {
        is_run_avai = false;
        break;
      }
      LOG_DEBUG("During compaction, clear SSTable file level:", level_no, ", run:", run_no);
      sstable_manager_->ClearSSTable(level_no, run_no);
    }
  }

  // 4. Copy compacted SSTables pages into the designated page
  LOG_DEBUG("During compaction, dump compacted memtable to level:", start_level_no, ", run:", 0);
  sstable_manager_->DumpSSTable(start_level_no /* level */, 0 /* run */, memtable);

  // 5. Remove the temporary SSTable file
  LOG_DEBUG("During compaction, remove temporary SSTable file");
  sstable_manager_->ClearTempSSTable();
}

}  // namespace ghostdb
