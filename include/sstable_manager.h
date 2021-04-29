//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// sstable_manager.h
//
// Identification: src/sstable_manager.h
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <map>
#include <vector>

#include "bloom.h"
#include "common.h"
#include "disk_manager.h"
#include "level.h"
#include "run.h"

namespace ghostdb {

class SSTableManager {
 public:
  SSTableManager(DiskManager *disk_manager);
  ~SSTableManager() = default;
  void ClearTempSSTable();
  void ClearSSTable(int level_no, int run_no);
  bool DumpSSTable(const std::map<Key, Val>& memtable);
  void DumpSSTable(int level, int run, const memtable_t& memtable);
  void DumpTempSSTable(const std::vector<std::pair<Key, Val>>& memtable);
  void LoadSSTable(int level_no, int run_no, Bloom *filter, memtable_t *memtable);
  bool IsEmpty(int level_no, int run_no) const;

 private:
  bool GetAvaiRun(int *level_no, int *run_no) const;

 private:
  /** manages permanent and temporary SSTable files */
  DiskManager *disk_manager_;
  /** DB holds at most MAX_LEVEL_NUM levels, each has upper limit of runs */
  std::vector<std::unique_ptr<Level>> levels_;
};

}  // namespace ghostdb
