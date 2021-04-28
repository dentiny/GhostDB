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

#include "common.h"

namespace ghostdb {

// string utils
std::string StringConcat(const char *base, const char *filename);
template<typename Cont>
std::string MemtableToString(const Cont& memtable);
std::string GetFilename(int level, int run);

// file system utils
void RemoveDirectory(const char *path);
void CreateDirectory(const char *path);
void InitDirectory(const char *path);
void RemoveFile(const char *path);
void CreateFile(const char *path);
void InitFile(const char *path);
void RenameFile(const char *old_name, const char *new_name);

// SSTable utils
memtable_t MergeSSTable(const memtable_t& new_sstable, const memtable_t& old_sstable);

}  // namespace ghostdb
