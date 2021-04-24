//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// logger.h
//
// Identification: src/logger.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#define LOG_DEBUG(msg) \
  Logger::Log(__FILE__, __LINE__, msg)

#include <iostream>

namespace ghostdb {

class Logger {
 public:
  static void Log(const char *filename, int line, const char *msg) {
    std::cout << filename << ":" << line << " " << msg << std::endl;
  }
};

}  // namespace ghostdb
