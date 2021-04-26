//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// config.h
//
// Identification: src/config.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <chrono>  // NOLINT
#include <cstdint>
#include <limits>
#include <string>

#include "common.h"

/* rounds up to the nearest multiple of WORD_SIZE */
#define ALIGNMENT sizeof(size_t)
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

namespace ghostdb {

/** base path for GhostDB */
static constexpr const char *const db_base = "/home/hj110/testdb/";
/** logging interval */
static constexpr std::chrono::duration<int64_t> LOG_TIMEOUT = std::chrono::seconds(1);
/** merge interval */
static constexpr std::chrono::duration<int64_t> MERGE_TIMEOUT = std::chrono::seconds(5);
/** value for Delete operation */
static constexpr Val TOMBSTOME = std::numeric_limits<Val>::min();
/** max level # */
static constexpr int MAX_LEVEL_NUM = 5;
/** # of run = MAX_RUN_PER_LEVEL * level # */
static constexpr int MAX_RUN_PER_LEVEL = 1;
/** # of kv pairs within buffer */
static constexpr int MAX_BUFFER_SIZE = 10;
/** # of kv pairs could dump into SSTable without compaction(debug util) */
static constexpr int KV_NUM_WITHOUT_COMPACT = MAX_LEVEL_NUM * (MAX_LEVEL_NUM + 1) * MAX_RUN_PER_LEVEL * MAX_BUFFER_SIZE / 2;
/** # of kv pairs in WAL buffer, used to flush WAL into disk periodically */
static constexpr int LOG_BUFFER_SIZE = 6 * sizeof(int32_t);
/** # of bits of bloom filter, accounts for 10% of valid bits(key-value pair) */
static constexpr int BLOOM_BITS = ALIGN(MAX_BUFFER_SIZE * RECORD_SIZE * 8 /* byte to bit */ / 10);

}  // namespace ghostdb
