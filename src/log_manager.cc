//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// log_manager.cc
//
// Identification: src/log_manager.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include <cassert>
#include <utility>

#include "coding.h"
#include "log_manager.h"

namespace ghostdb {

LogManager::LogManager() :
  disk_manager_(std::make_unique<DiskManager>()),
  enable_logging_(true),
  request_flush_(false),
  log_buffer_size_(0),
  flush_buffer_size_(0),
  log_buffer_(new char[LOG_BUFFER_SIZE]),
  flush_buffer_(new char[LOG_BUFFER_SIZE]) {
  RunFlushThread();
}

LogManager::~LogManager() {
  StopFlushThread();
  delete log_buffer_;
  delete flush_buffer_;
  log_buffer_ = nullptr;
  flush_buffer_ = nullptr;
}

/*
 * Conditions for flushing WAL:
 * (1) wait period exceeds LOG_TIMEOUT
 * (2) no sufficient space for in-coming record(invoked within AppendLogRecord)
 * (3) StopFlushThread invoke Flush, then blocks until flush completes 
 */
void LogManager::RunFlushThread() {
  LOG_DEBUG("Start WAL flush thread");
  flush_future_ = std::async([&]() {
    while (enable_logging_) {
      std::unique_lock<std::mutex> lck(latch_);
      flush_cv_.wait_for(lck, LOG_TIMEOUT, [&]() { return request_flush_.load(); });
      assert(flush_buffer_size_ == 0);
      if (log_buffer_size_ > 0) {
        std::swap(log_buffer_size_, flush_buffer_size_);
        std::swap(log_buffer_, flush_buffer_);
        LOG_DEBUG("Try to flush WAL");
        disk_manager_->WriteLog(flush_buffer_, flush_buffer_size_);
        flush_buffer_size_ = 0;
      }
      request_flush_ = false;
      append_cv_.notify_all();
    }
  });
}

void LogManager::Flush() {
  std::unique_lock<std::mutex> lck(latch_);
  request_flush_ = true;
  flush_cv_.notify_one();
  append_cv_.wait(lck, [&]() { return !request_flush_; });  // block until flush completes
}

void LogManager::StopFlushThread() {
  Flush();
  enable_logging_ = false;
  flush_future_.get();
  assert(log_buffer_size_ == 0);
  assert(flush_buffer_size_ == 0);
}

void LogManager::AppendLogRecord(int32_t key, int32_t val) {
  std::unique_lock<std::mutex> lck(latch_);
  if (log_buffer_size_ + RECORD_SIZE >= LOG_BUFFER_SIZE) {
    request_flush_ = true;
    flush_cv_.notify_one();
    append_cv_.wait(lck, [&]() { return log_buffer_size_ + RECORD_SIZE < LOG_BUFFER_SIZE; });
  }
  uint64_t record = (static_cast<uint32_t>(key) << 4) | (static_cast<uint32_t>(val));
  EncodeFixed64(log_buffer_ + log_buffer_size_, record);
  log_buffer_size_ += RECORD_SIZE;
}

}  // namespace ghostdb
