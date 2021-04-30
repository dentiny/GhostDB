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
#include "consts.h"

/* rounds up to the nearest multiple of WORD_SIZE */
#define ALIGNMENT sizeof(size_t)
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

namespace ghostdb {

/** base path for GhostDB */
static constexpr const char *const db_base = "/home/hj110/testdb/";
/** logging interval */
static constexpr std::chrono::duration<int64_t> LOG_TIMEOUT = std::chrono::seconds(1);
/** major compaction interval */
static constexpr std::chrono::duration<int64_t> COMPACTION_TIMEOUT = std::chrono::seconds(5);
/** value for Delete operation */
static constexpr Val TOMBSTOME = std::numeric_limits<Val>::min();
/** minor compaction merges uppermost two levels */
static constexpr int MINOR_COMPACTION_LEVEL_NUM = 2;
/** magic number in page header */
static constexpr uint8_t PAGE_HEADER_MAGIC = 0xAA;  // 0b1010,1010
/** max level # */
static constexpr int MAX_LEVEL_NUM = 5;
/** # of run = MAX_RUN_PER_LEVEL * level # */
static constexpr int MAX_RUN_PER_LEVEL = 1;
/** # of kv pairs within buffer(memtable) */
static constexpr int MAX_BUFFER_SIZE = 5;
/** # of biggest runs in a level, used to calculate sstable_id(reference common.h) */
static constexpr int MAX_RUN_IN_LEVEL = MAX_LEVEL_NUM * MAX_RUN_PER_LEVEL;
/** # of pages of non-overflow pages */
static constexpr int TOTAL_NON_OVERFLOW_PAGES = (1 + MAX_LEVEL_NUM) * MAX_LEVEL_NUM * MAX_RUN_PER_LEVEL / 2;
/** # of all possible runs, used to fix the size of ClockReplacer */
static constexpr int SSTABLE_NUM = MAX_RUN_IN_LEVEL * MAX_LEVEL_NUM;
/** # of kv pairs in WAL buffer, used to flush WAL into disk periodically */
static constexpr int LOG_BUFFER_SIZE = 6 * sizeof(int32_t);
/** # of bits of bloom filter */
static constexpr int BLOOM_BITS = 64;

/** # of bits of bloom filter, accounts for 10% of valid bits(key-value pair) */
// static constexpr int BLOOM_BITS = ALIGN(MAX_BUFFER_SIZE * RECORD_SIZE * 8 /* byte to bit */ / 10);  // 64 bit

}  // namespace ghostdb
