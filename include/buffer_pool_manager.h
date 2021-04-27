//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// buffer_pool_manager.h
//
// Identification: src/buffer_pool_manager.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include "disk_manager.h"

namespace ghostdb {

class BufferPoolManager {
 public:
  BufferPoolManager(int pool_size, DiskManager *disk_manager);

 private:
  int pool_size_;
  DiskManager *disk_manager_;
};

}  // namespace ghostdb
