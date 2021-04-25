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

#pragma once

#include <atomic>
#include <condition_variable>  // NOLINT
#include <future>  // NOLINT
#include <memory>
#include <mutex>  // NOLINT

#include "config.h"
#include "disk_manager.h"

namespace ghostdb {

class LogManager {
 public:
  LogManager();
  ~LogManager() noexcept;
  void AppendLogRecord(int32_t key, int32_t val);

 private:
  void Flush();
  void RunFlushThread();
  void StopFlushThread();

 private:
  std::unique_ptr<DiskManager> disk_manager_;
  std::atomic<bool> enable_logging_;
  std::atomic<bool> request_flush_;
  static constexpr int RECORD_SIZE = 2 * sizeof(int32_t);
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
