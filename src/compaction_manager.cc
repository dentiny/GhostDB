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

#include "compaction_manager.h"

using std::make_unique;

namespace ghostdb {

CompactionManager::CompactionManager() :
  disk_manager_(make_unique<DiskManager>(true /* is_compaction */)),
  enable_compaction_(true) {
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
void CompactionManager::LaunchMinorCompaction() {
  LOG_DEBUG("Launch minor compaction");
}

// Invoked with latch_ held.
void CompactionManager::LaunchMajorCompaction() {
  LOG_DEBUG("Launch major compaction");
}

}  // namespace ghostdb
