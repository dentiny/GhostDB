//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// log_manager.h
//
// Identification: src/log_manager.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

/*
 * Conditions for flushing WAL:
 * (1) wait period exceeds LOG_TIMEOUT
 * (2) no sufficient space for in-coming record(invoked within AppendLogRecord)
 * (3) GhostDB explicitly invoked Flush when memtable full, have to flush WAL before persisting
 */

#pragma once

#include <atomic>
#include <condition_variable>  // NOLINT
#include <future>  // NOLINT
#include <memory>
#include <mutex>  // NOLINT

#include "common.h"
#include "config.h"
#include "disk_manager.h"

namespace ghostdb {

class LogManager {
 public:
  LogManager();
  ~LogManager() noexcept;
  void Flush();
  void AppendLogRecord(Key key, Val val);

 private:
  void RunFlushThread();
  void StopFlushThread();

 private:
  std::unique_ptr<DiskManager> disk_manager_;
  std::atomic<bool> enable_logging_;
  std::atomic<bool> request_flush_;
  int log_buffer_size_;
  int flush_buffer_size_;
  char *log_buffer_;  // buffer to apppend
  char *flush_buffer_;  // buffer to flush to disk

  std::mutex latch_;
  std::condition_variable flush_cv_;
  std::condition_variable append_cv_;
  std::future<void> flush_future_;
};

}  // namespace ghostdb
