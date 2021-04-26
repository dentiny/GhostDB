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

#include <map>
#include <string>

namespace ghostdb {

std::string StringConcat(const char *base, const char *filename);

std::string MemtableToString(const std::map<Key, Val>& memtable);

}  // namespace ghostdb
