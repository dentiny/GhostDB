//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// util.cc
//
// Identification: src/util.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "common.h"
#include "logger.h"
#include "util.h"

#define INIT_CMD_LENGTH 100

using std::map;
using std::string;
using std::vector;

namespace ghostdb {

string StringConcat(const char *base, const char *filename) {
  string file = string(base) + string(filename);
  return file;
}

string MemtableToString(const map<Key, Val>& memtable) {
  vector<char> arr(RECORD_SIZE * memtable.size());
  size_t offset = 0;
  for (auto& [key, val] : memtable) {
    memmove(arr.data() + offset, &key, sizeof(key));
    offset += static_cast<int>(sizeof(key));
    memmove(arr.data() + offset, &val, sizeof(val));
    offset += static_cast<int>(sizeof(val));
  }
  std::string s(arr.begin(), arr.end());
  return s;
}

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
  LOG_DEBUG("Create directory:", path);
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
