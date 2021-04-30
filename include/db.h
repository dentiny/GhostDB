//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// db.h
//
// Identification: src/db.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

/*
 * There're three files related to GhostDB:
 * 1. memtable.log
 * 2. sstable.db
 * 3. sstable.temp
 */

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "buffer.h"
#include "common.h"
#include "compaction_manager.h"
#include "config.h"
#include "disk_manager.h"
#include "level.h"
#include "logger.h"
#include "log_manager.h"
#include "sstable_manager.h"

namespace ghostdb {

class GhostDB {
 public:
  GhostDB();
  ~GhostDB() noexcept;
  bool Put(Key key, Val val);
  bool Get(Key key, Val *val);
  bool Delete(Key key);

 private:
  bool GetAvaiRun(int *level_no, int *run_no) const;

 private:
  /** DB owns one single DiskManager for both WAL and SSTable */
  std::unique_ptr<DiskManager> disk_manager_;
  /** DB holds exactly one currently-using buffer */
  std::unique_ptr<Buffer> buffer_;
  /** manages SSTable dumping and loading */
  std::unique_ptr<SSTableManager> sstable_manager_;
  /** make minor and major compactions */
  std::unique_ptr<CompactionManager> compaction_manager_;
};

}  // namespace ghostdb
