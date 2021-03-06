//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// common.h
//
// Identification: src/common.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cstdint>
#include <map>
#include <utility>
#include <vector>

namespace ghostdb {

using Key = int32_t;
using Val = int32_t;
using MappingType = std::pair<Key, Val>;
static constexpr int RECORD_SIZE = sizeof(Key) + sizeof(Val);

/**
 * How sstable_id is calculated:
 * MAX_RUN_IN_LEVEL = MAX_LEVEL_NUM * MAX_RUN_PER_LEVEL
 * sstable_id = MAX_RUN_IN_LEVEL * level + run
 */
using sstable_id_t = int;

/*
 * When reading key-value pair from storage, since they're already sorted while dumping,
 * keeping them in the vector is enough for merging and binary search.
 */
using buffer_t = std::map<Key, Val>;
using sstable_t = std::vector<MappingType>;

}  // namespace ghostdb
