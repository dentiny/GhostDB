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
#include "compaction_manager.h"
#include "config.h"
#include "disk_manager.h"
#include "sstable_manager.h"

namespace ghostdb {

class RecoveryManager {
 public:
  RecoveryManager(DiskManager *disk_manager,
                  SSTableManager *sstable_manager,
                  CompactionManager *compaction_manager);
  ~RecoveryManager() = default;
  void PerformRecover();

 private:
  void RecoverSSTable();
  void RecoverWAL();

 private:
  /** directly read and write files on disk */
  DiskManager *disk_manager_;
  /** dump WAL into the SSTable file */
  SSTableManager *sstable_manager_;
  /** dumping WAL may need minor compaction */
  CompactionManager *compaction_manager_;
};

}  // namespace ghostdb
