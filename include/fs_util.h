//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// fs_util.h
//
// Identification: src/fs_util.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

namespace ghostdb {

void RemoveDirectory(const char *path);

void CreateDirectory(const char *path);

void InitDirectory(const char *path);

void RemoveFile(const char *path);

void CreateFile(const char *path);

void InitFile(const char *path);

}  // namespace ghostdb
