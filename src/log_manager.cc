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

#include "log_manager.h"

namespace ghostdb {

LogManager::LogManager(int level, int run) :
  disk_manager_(std::make_unique<DiskManager>(level, run)),
  log_buffer_(new char[LOG_BUFFER_SIZE]),
  flush_buffer_(new char[LOG_BUFFER_SIZE]) {
  RunFlushThread();
}

LogManager::~LogManager() {
  delete log_buffer_;
  delete flush_buffer_;
}

void LogManager::RunFlushThread() {

}

void LogManager::StopFlushThread() {

}

void LogManager::AppendLogRecord() {

}

}  // namespace ghostdb
