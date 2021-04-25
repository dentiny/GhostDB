//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// coding.h
//
// Identification: src/coding.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cstdint>

namespace ghostdb {

void EncodeFixed32(char *dst, uint32_t value);
void EncodeFixed64(char *dst, uint64_t value);

}  // namespace ghostdb
