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
  for (Key key = 0; key < 75; ++key) {
    Val val = key;
    assert(db->Put(key, val));
  }
  for (Key key = 0; key < 75; ++key) {
    Val expected = key;
    Val val;
    assert(db->Get(key, &val));
    assert(val == expected);
  }
  buffer_t res;
  db->GetRange(30, 59, &res);
  assert(res.size() == 30);
}

}  // namespace ghostdb

int main(int argc, char **argv) {
  ghostdb::SingleThreadTest();
  return EXIT_SUCCESS;
}
