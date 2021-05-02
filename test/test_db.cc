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

#include <memory>
#include <gtest/gtest.h>

#include "common.h"
#include "db.h"

using namespace ghostdb;
using namespace std;

TEST(SingleThreadTest, SimpleTest) {
  // max key-value pairs without compaction: 160
  unique_ptr<GhostDB> db = make_unique<GhostDB>();
  for (Key key = 0; key < 75; ++key) {
    Val val = key;
    EXPECT_TRUE(db->Put(key, val));
  }
  for (Key key = 0; key < 75; ++key) {
    Val expected = key;
    Val val;
    EXPECT_TRUE(db->Get(key, &val));
    EXPECT_EQ(val, expected);
  }
  buffer_t res;
  db->GetRange(30, 59, &res);
  EXPECT_EQ(res.size(), 30);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
