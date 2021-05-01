//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// rwlatch.h
//
// Identification: src/rwlatch.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <climits>
#include <condition_variable>  // NOLINT
#include <mutex>  // NOLINT

#include "macro.h"

namespace ghostdb {

class ReaderWriterLatch {
  using mutex_t = std::mutex;
  using cond_t = std::condition_variable;
  static const uint32_t MAX_READERS = UINT_MAX;

 public:
  ReaderWriterLatch() = default;
  ~ReaderWriterLatch() { std::lock_guard<mutex_t> guard(mutex_); }

  DISALLOW_COPY(ReaderWriterLatch)

  /**
   * Acquire a write latch.
   */
  void WLock() {
    std::unique_lock<mutex_t> latch(mutex_);
    while (writer_entered_) {
      reader_.wait(latch);
    }
    writer_entered_ = true;
    while (reader_count_ > 0) {
      writer_.wait(latch);
    }
  }

  /**
   * Release a write latch.
   */
  void WUnlock() {
    std::lock_guard<mutex_t> guard(mutex_);
    writer_entered_ = false;
    reader_.notify_all();
  }

  /**
   * Acquire a read latch.
   */
  void RLock() {
    std::unique_lock<mutex_t> latch(mutex_);
    while (writer_entered_ || reader_count_ == MAX_READERS) {
      reader_.wait(latch);
    }
    reader_count_++;
  }

  /**
   * Release a read latch.
   */
  void RUnlock() {
    std::lock_guard<mutex_t> guard(mutex_);
    reader_count_--;
    if (writer_entered_) {
      if (reader_count_ == 0) {
        writer_.notify_one();
      }
    } else {
      if (reader_count_ == MAX_READERS - 1) {
        reader_.notify_one();
      }
    }
  }

  bool IsWriteLocked() const {
    std::lock_guard<mutex_t> lck(mutex_);
    return writer_entered_ && reader_count_ == 0;
  }

  bool IsReadLocked() const {
    std::lock_guard<mutex_t> lck(mutex_);
    return reader_count_ > 0;
  }

  bool IsLocked() const {
    std::lock_guard<mutex_t> lck(mutex_);
    bool is_read_locked = reader_count_ > 0;
    bool is_write_locked = writer_entered_ && reader_count_ == 0;
    return is_read_locked || is_write_locked;
  }

 private:
  mutable mutex_t mutex_;
  cond_t writer_;
  cond_t reader_;
  uint32_t reader_count_{0};
  bool writer_entered_{false};
};

}  // namespace ghostdb
