//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// test_db.h
//
// Identification: src/test_db.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <memory>

#include "common.h"
#include "db.h"

using std::make_unique;
using std::unique_ptr;

namespace ghostdb {

void SingleThreadTest() {
  // max key-value pairs without compaction: 160
  unique_ptr<GhostDB> db = make_unique<GhostDB>();
  for (Key key = 0; key < 161; ++key) {
    Val val = key;
    assert(db->Put(key, val));
  }
}

}  // namespace ghostdb

int main(int argc, char **argv) {
  ghostdb::SingleThreadTest();
  return EXIT_SUCCESS;
}
