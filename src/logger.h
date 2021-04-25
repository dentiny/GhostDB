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

#define LOG_DEBUG(...) \
  Logger::Debug(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) \
  Logger::Error(__FILE__, __LINE__, __VA_ARGS__)

#include <cstdlib>
#include <iostream>
#include <string>

namespace ghostdb {

class Logger {
 public:
  template<typename ... Args>
  static void Debug(const char *filename, int line, Args ... args) {
    std::cerr << filename << ":" << line << " ";
    Impl(args...);
  }

  template<typename ... Args>
  static void Error(const char *filename, int line, Args ... args) {
    std::cerr << filename << ":" << line << " ";
    Impl(args...);
    exit(EXIT_FAILURE);
  }

 private:
  template<typename T>
  static void Impl(T arg) {
    std::cerr << arg << std::endl;
  }

  template<typename T, typename ... Args>
  static void Impl(T arg, Args ... args) {
    std::cerr << arg;
    Impl(args...);
  }
};

}  // namespace ghostdb
