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

namespace ghostdb {

using Key = int32_t;
using Val = int32_t;
static constexpr int RECORD_SIZE = sizeof(Key) + sizeof(Val);

/**
 * How sstable_id is calculated:
 * MAX_RUN_IN_LEVEL = MAX_LEVEL_NUM * MAX_RUN_PER_LEVEL
 * sstable_id = MAX_RUN_IN_LEVEL * level + run
 */
using sstable_id_t = int;

}  // namespace ghostdb
