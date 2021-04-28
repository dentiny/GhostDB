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

void Bloom::SetKey(const Key& key) {
  filter_.set(Hash1(key));
  filter_.set(Hash2(key));
  filter_.set(Hash3(key));
}

bool Bloom::CouldHasKey(const Key& key) const {
  return filter_.test(Hash1(key)) &&
         filter_.test(Hash2(key)) &&
         filter_.test(Hash3(key));
}

void Bloom::ClearKey() {
  filter_.reset();
}

uint64_t Bloom::ToInt() const {
  return static_cast<uint64_t>(filter_.to_ulong());
}

string Bloom::ToString() const {
  return filter_.to_string();
}

uint64_t Bloom::Hash1(const Key& k) const {
  uint64_t key = static_cast<uint64_t>(k);
  key = ~key + (key << 15);
  key = key ^ (key >> 12);
  key = key + (key << 2);
  key = key ^ (key >> 4);
  key = key * 2057;
  key = key ^ (key >> 16);
  return key % BLOOM_BITS;
}

uint64_t Bloom::Hash2(const Key& k) const {
  uint64_t key = static_cast<uint64_t>(k);
  key = (key + 0x7ed55d16) + (key << 12);
  key = (key ^ 0xc761c23c) ^ (key >> 19);
  key = (key + 0x165667b1) + (key << 5);
  key = (key + 0xd3a2646c) ^ (key << 9);
  key = (key + 0xfd7046c5) + (key << 3);
  key = (key ^ 0xb55a4f09) ^ (key >> 16);
  return key % BLOOM_BITS;
}

uint64_t Bloom::Hash3(const Key& k) const {
  uint64_t key = static_cast<uint64_t>(k);
  key = (key ^ 61) ^ (key >> 16);
  key = key + (key << 3);
  key = key ^ (key >> 4);
  key = key * 0x27d4eb2d;
  key = key ^ (key >> 15);
  return key % BLOOM_BITS;
}

}  // namespace ghostdb
