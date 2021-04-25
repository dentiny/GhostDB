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

#include "db.h"

using std::make_unique;
using std::unique_ptr;

namespace ghostdb {

void SingleThreadTest() {
  unique_ptr<GhostDB> db = make_unique<GhostDB>();
  assert(db->Put(1, 2));
  int32_t val;
  assert(db->Get(1, &val));
  assert(val == 2);
  assert(db->Delete(1));
  assert(!db->Get(1, &val));
}

}  // namespace ghostdb

int main(int argc, char **argv) {
  ghostdb::SingleThreadTest();
  return EXIT_SUCCESS;
}
