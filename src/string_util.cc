//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// string_util.cc
//
// Identification: src/string_util.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include <cstring>
#include <vector>

#include "common.h"
#include "string_util.h"

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

}  // namespace ghostdb
