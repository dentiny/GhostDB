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
  Bloom() { filter_ = std::bitset<BLOOM_BITS>(0); }
  Bloom(uint64_t filter) { filter_ = std::bitset<BLOOM_BITS>(filter); }  // FIXME: hard-code bit #
  ~Bloom() = default;
  bool operator==(const Bloom& rhs) { return filter_ == rhs.filter_; }
  void SetKey(const Key& key);
  bool CouldHasKey(const Key& key) const;
  void ClearKey();
  uint64_t ToInt() const;
  std::string ToString() const;

 private:
  uint64_t Hash1(const Key& k) const;
  uint64_t Hash2(const Key& k) const;
  uint64_t Hash3(const Key& k) const;

 private:
  std::bitset<BLOOM_BITS> filter_;
};

}  // namespace ghostdb
