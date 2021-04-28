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
#include <utility>
#include <vector>

#include "config.h"
#include "logger.h"
#include "util.h"

#define INIT_CMD_LENGTH 100

using std::map;
using std::pair;
using std::string;
using std::vector;

namespace ghostdb {

static void CheckStringOverflow(const char *cmd) {
  if (cmd[INIT_CMD_LENGTH - 1] != 0) {
    LOG_ERROR("Directory or file path exceeds expected length.");
  }
}

string StringConcat(const char *base, const char *filename) {
  string file = string(base) + string(filename);
  return file;
}

template<typename Cont>
string MemtableToString(const Cont& memtable) {
  vector<char> arr(RECORD_SIZE * memtable.size());
  size_t offset = 0;
  for (auto& [key, val] : memtable) {
    memmove(arr.data() + offset, &key, sizeof(key));
    offset += static_cast<int>(sizeof(key));
    memmove(arr.data() + offset, &val, sizeof(val));
    offset += static_cast<int>(sizeof(val));
  }
  string s(arr.begin(), arr.end());
  return s;
}

// Explicit instantiation to export symbol.
template string MemtableToString(const vector<pair<Key, Val>>& memtable);
template string MemtableToString(const map<Key, Val>& memtable);

string GetFilename(int level, int run) {
  int db_base_len = strlen(db_base);
  char filename[INIT_CMD_LENGTH] = { 0 };
  memmove(filename, db_base, db_base_len);
  snprintf(filename + db_base_len, sizeof(filename) - db_base_len, "level_%d_run_%d.db", level, run);
  CheckStringOverflow(filename);
  return string(filename);
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

void RenameFile(const char *old_name, const char *new_name) {
  char cmd[INIT_CMD_LENGTH] = { 0 };
  snprintf(cmd, sizeof(cmd), "mv %s %s", old_name, new_name);
  CheckStringOverflow(cmd);
  system(cmd);
}

memtable_t MergeSSTable(const memtable_t& new_sstable, const memtable_t& old_sstable) {
  memtable_t merged_sstable;
  size_t size1 = new_sstable.size();
  size_t size2 = old_sstable.size();
  size_t idx1 = 0;
  size_t idx2 = 0;
  while (idx1 < size1 || idx2 < size2) {
    if (idx1 == size1) {
      merged_sstable.emplace_back(old_sstable[idx2++]);
      continue;
    } else if (idx2 == size2) {
      merged_sstable.emplace_back(new_sstable[idx1++]);
      continue;
    }
    Key new_key = new_sstable[idx1].first;
    Key old_key = old_sstable[idx2].first;
    if (new_key < old_key) {
      merged_sstable.emplace_back(new_sstable[idx1++]);
    } else if (old_key < new_key) {
      merged_sstable.emplace_back(old_sstable[idx2++]);
    } else {
      if (new_key != TOMBSTOME) {
        merged_sstable.emplace_back(new_sstable[idx1]);
      }
      ++idx1;
      ++idx2;
    }
  }
  return merged_sstable;
}

}  // namespace ghostdb
