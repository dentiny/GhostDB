//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// string_util.h
//
// Identification: src/string_util.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <string>

namespace ghostdb {

std::string StringConcat(const char *base, const char *filename) {
  std::string file = std::string(base) + std::string(filename);
  return file;
}

}  // namespace ghostdb
