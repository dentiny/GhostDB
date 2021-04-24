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
  Logger::Debug(__FILE__, __LINE__, msg)
#define LOG_ERROR(msg) \
  Logger::Error(__FILE__, __LINE__, msg)

#include <cstdlib>
#include <iostream>
#include <string>

namespace ghostdb {

class Logger {
 public:
  static void Debug(const char *filename, int line, const char *msg) {
    std::cout << filename << ":" << line << " " << msg << std::endl;
  }

  static void Debug(const char *filename, int line, const std::string& msg) {
    std::cout << filename << ":" << line << " " << msg << std::endl;
  }

  static void Error(const char *filename, int line, const char *msg) {
    std::cout << filename << ":" << line << " " << msg << std::endl;
    exit(EXIT_FAILURE);
  }

  static void Error(const char *filename, int line, const std::string& msg) {
    std::cout << filename << ":" << line << " " << msg << std::endl;
    exit(EXIT_FAILURE);
  }
};

}  // namespace ghostdb
