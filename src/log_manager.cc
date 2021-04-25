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

#include "log_manager.h"

namespace ghostdb {

LogManager::LogManager(int level, int run) :
  disk_manager_(std::make_unique<DiskManager>(level, run)),
  enable_logging_(true),
  request_flush_(false),
  log_buffer_size_(0),
  flush_buffer_size_(0),
  log_buffer_(new char[LOG_BUFFER_SIZE]),
  flush_buffer_(new char[LOG_BUFFER_SIZE]) {
  RunFlushThread();
}

LogManager::~LogManager() {
  delete log_buffer_;
  delete flush_buffer_;
  log_buffer_ = nullptr;
  flush_buffer_ = nullptr;
  StopFlushThread();
}

void LogManager::RunFlushThread() {
  flush_future_ = std::async([&]() {
    while (enable_logging_) {
      std::unique_lock<std::mutex> lck(latch_);
      flush_cv_.wait_for(lck, LOG_TIMEOUT, [&]() { return request_flush_.load(); });
      assert(flush_buffer_size_ == 0);
      if (log_buffer_size_ > 0) {
        std::swap(log_buffer_size_, flush_buffer_size_);
        std::swap(log_buffer_, flush_buffer_);
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

void LogManager::AppendLogRecord() {

}

}  // namespace ghostdb
