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
#include <string>

namespace ghostdb {

static constexpr const char *const db_base = "/home/hj110/testdb/";
static constexpr std::chrono::duration<int64_t> LOG_TIMEOUT = std::chrono::seconds(10);
static constexpr int32_t TOMBSTOME = INT32_MIN;
static constexpr uint32_t MAX_LEVEL_NUM = 5;
/** # of kv pairs within buffer */
static constexpr uint32_t MAX_BUFFER_SIZE = 10;
/** # of kv pairs in WAL buffer */
static constexpr int LOG_BUFFER_SIZE = 4 * sizeof(int32_t);

}  // namespace ghostdb
