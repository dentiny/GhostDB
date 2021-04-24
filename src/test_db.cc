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

#include "db.h"

namespace ghostdb {

void SingleThreadTest() {
  GhostDB *db = new GhostDB();
  assert(GhostDB::Open("/home/hj110/test.db", &db));
  assert(db->Put(1, 2));
  int32_t val;
  assert(db->Get(1, &val));
  assert(val == 2);
  assert(db->Delete(1));
  assert(!db->Get(1, &val));
  delete db;
}

}  // namespace ghostdb

int main(int argc, char **argv) {
  ghostdb::SingleThreadTest();
  return EXIT_SUCCESS;
}
