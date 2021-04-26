//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// fs_util.cc
//
// Identification: src/fs_util.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include <cstdio>
#include <cstdlib>

#include "fs_util.h"

namespace ghostdb {

void RemoveDirectory(const char *path) {
  char cmd[100] = { 0 };
  snprintf(cmd, sizeof(cmd), "rm -rf %s", path);
  system(cmd);
}

void CreateDirectory(const char *path) {
  char cmd[100] = { 0 };
  snprintf(cmd, sizeof(cmd), "mkdir -p %s", path);
  system(cmd);
}

void InitDirectory(const char *path) {
  RemoveDirectory(path);
  CreateDirectory(path);
}

}  // namespace ghostdb
