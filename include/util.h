//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// util.h
//
// Identification: src/util.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <map>
#include <string>

namespace ghostdb {

// string utils
std::string StringConcat(const char *base, const char *filename);
std::string MemtableToString(const std::map<Key, Val>& memtable);

// file system utils
void RemoveDirectory(const char *path);
void CreateDirectory(const char *path);
void InitDirectory(const char *path);
void RemoveFile(const char *path);
void CreateFile(const char *path);
void InitFile(const char *path);

}  // namespace ghostdb
