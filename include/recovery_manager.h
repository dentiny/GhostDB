//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// recovery_manager.h
//
// Identification: src/recovery_manager.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common.h"
#include "config.h"
#include "disk_manager.h"

namespace ghostdb {

class RecoveryManager {
 public:
  RecoveryManager(DiskManager *disk_manager);
  ~RecoveryManager() = default;
  void PerformRecover();

 private:
  void RecoverSSTable();
  void RecoverWAL();

 private:
  DiskManager *disk_manager_;
};

}  // namespace ghostdb
