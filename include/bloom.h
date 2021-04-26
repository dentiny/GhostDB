//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// bloom.h
//
// Identification: src/bloom.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <bitset>
#include <cstdint>
#include <map>
#include <string>

#include "config.h"

namespace ghostdb {

class Bloom {
 public:
  Bloom() = default;
  ~Bloom() = default;
  void SetKey(Key key);
  void ClearKey();
  std::string ToString() const;

 private:
  uint64_t hash1(Key k) const;
  uint64_t hash2(Key k) const;
  uint64_t hash3(Key k) const;

 private:
  std::bitset<BLOOM_BITS> filter_;
};

}  // namespace ghostdb
