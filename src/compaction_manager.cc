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
      major_compaction_cv_.wait_for(lck, COMPACTION_TIMEOUT, [&]{ return request_minor_compaction_.load(); });
      if (request_minor_compaction_) {
        LOG_DEBUG("Try minor compaction");
      } else {
        LOG_DEBUG("Try major compaction");
      }
      // TODO: compaction
      request_minor_compaction_ = false;
    }
  });
}

void CompactionManager::StopCompactionThread() {
  RequestMajorCompaction();
  enable_compaction_ = false;
  compaction_future_.get();
}

void CompactionManager::RequestMinorCompaction() {

}

void CompactionManager::RequestMajorCompaction() {

}

void CompactionManager::LaunchMinorCompaction() {

}

void CompactionManager::LaunchMajorCompaction() {

}

}  // namespace ghostdb
