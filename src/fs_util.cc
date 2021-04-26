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
#include "logger.h"

#define INIT_CMD_LENGTH 100

namespace ghostdb {

static void CheckStringOverflow(const char *cmd) {
  if (cmd[INIT_CMD_LENGTH - 1] != 0) {
    LOG_ERROR("Directory or file path exceeds expected length.");
  }
}

void RemoveDirectory(const char *path) {
  char cmd[INIT_CMD_LENGTH] = { 0 };
  snprintf(cmd, sizeof(cmd), "rm -rf %s", path);
  CheckStringOverflow(cmd);
  system(cmd);
}

void CreateDirectory(const char *path) {
  char cmd[INIT_CMD_LENGTH] = { 0 };
  snprintf(cmd, sizeof(cmd), "mkdir -p %s", path);
  CheckStringOverflow(cmd);
  system(cmd);
}

void InitDirectory(const char *path) {
  RemoveDirectory(path);
  CreateDirectory(path);
}

void RemoveFile(const char *path) {
  char cmd[INIT_CMD_LENGTH] = { 0 };
  snprintf(cmd, sizeof(cmd), "rm -f %s", path);
  CheckStringOverflow(cmd);
  system(cmd);
}

void CreateFile(const char *path) {
  char cmd[INIT_CMD_LENGTH] = { 0 };
  snprintf(cmd, sizeof(cmd), "touch %s", path);
  CheckStringOverflow(cmd);
  system(cmd);
}

void InitFile(const char *path) {
  RemoveFile(path);
  CreateFile(path);
}

}  // namespace ghostdb
