//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// consts.h
//
// Identification: src/consts.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once 

namespace ghostdb {

/** page size */
static constexpr int PAGE_SIZE = 1024;
/** zero page to dump into storage */
static const char zero_data[PAGE_SIZE] = { 0 };

}  // namespace ghostdb
