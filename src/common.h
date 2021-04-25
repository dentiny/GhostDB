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

}  // namespace ghostdb
