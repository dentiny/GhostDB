//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// bloom.cc
//
// Identification: src/bloom.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include "bloom.h"

using std::string;

namespace ghostdb {

void Bloom::SetKey(Key key) {
  filter_.set(hash1(key));
  filter_.set(hash2(key));
  filter_.set(hash3(key));
}

void Bloom::ClearKey() {
  filter_.reset();
}

string Bloom::ToString() const {
  return filter_.to_string();
}

uint64_t Bloom::hash1(Key k) const {
  uint64_t key = static_cast<uint64_t>(k);
  key = ~key + (key << 15);
  key = key ^ (key >> 12);
  key = key + (key << 2);
  key = key ^ (key >> 4);
  key = key * 2057;
  key = key ^ (key >> 16);
  return key % BLOOM_BITS;
}

uint64_t Bloom::hash2(Key k) const {
  uint64_t key = static_cast<uint64_t>(k);
  key = (key + 0x7ed55d16) + (key << 12);
  key = (key ^ 0xc761c23c) ^ (key >> 19);
  key = (key + 0x165667b1) + (key << 5);
  key = (key + 0xd3a2646c) ^ (key << 9);
  key = (key + 0xfd7046c5) + (key << 3);
  key = (key ^ 0xb55a4f09) ^ (key >> 16);
  return key % BLOOM_BITS;
}

uint64_t Bloom::hash3(Key k) const {
  uint64_t key = static_cast<uint64_t>(k);
  key = (key ^ 61) ^ (key >> 16);
  key = key + (key << 3);
  key = key ^ (key >> 4);
  key = key * 0x27d4eb2d;
  key = key ^ (key >> 15);
  return key % BLOOM_BITS;
}

}  // namespace ghostdb
