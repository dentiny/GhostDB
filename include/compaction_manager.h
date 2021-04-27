//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// compaction_manager.h
//
// Identification: src/compaction_manager.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

/*
 * Condition for compaction:
 * (1) major compaction: wait interval exceeds COMPACTION_TIMEOUT
 * (2) minor compaction: there's no available run, GhostDb requests for minor compaction
 * (3) major compaction: GhostDb exits, StopCompactionThread is invoked
 */

#pragma once

#include <atomic>
#include <condition_variable>  // NOLINT
#include <future>  // NOLINT
#include <mutex>  // NOLINT

namespace ghostdb {

class CompactionManager {
 public:
  CompactionManager();
  ~CompactionManager();
  void RequestMinorCompaction();

 private:
  void RunCompactionThread();
  void StopCompactionThread();
  void RequestMajorCompaction();
  void LaunchMinorCompaction();
  void LaunchMajorCompaction();

 private:
  std::atomic<bool> enable_compaction_;
  std::atomic<bool> request_minor_compaction_;
  std::atomic<bool> request_major_compaction_;

  std::mutex latch_;
  std::condition_variable compaction_cv_;
  std::condition_variable wait_compaction_cv_;
  std::future<void> compaction_future_;
};

}  // namespace ghostdb
