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
  unique_ptr<GhostDB> db = make_unique<GhostDB>();
  for (Key key = 0; key < 10; ++key) {
    Val val = key;
    assert(db->Put(key, val));
  }
  for (Key key = 0; key < 10; ++key) {
    Val val;
    assert(db->Get(key, &val));
    Val expected = key;
    assert(val == expected);
  }
  for (Key key = 11; key < 20; ++key) {
    Val val = key;
    assert(!db->Put(key, val));
  }
}

}  // namespace ghostdb

int main(int argc, char **argv) {
  ghostdb::SingleThreadTest();
  return EXIT_SUCCESS;
}
