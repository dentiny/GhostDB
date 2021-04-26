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

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "buffer.h"
#include "common.h"
#include "compaction_manager.h"
#include "config.h"
#include "level.h"
#include "logger.h"
#include "log_manager.h"

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
  /** make minor and major compactions */
  std::unique_ptr<CompactionManager> compaction_manager_;
  /** DB holds exactly one currently-using buffer */
  std::unique_ptr<Buffer> buffer_;
  /** DB holds at most MAX_LEVEL_NUM levels, each has upper limit of runs */
  std::vector<std::unique_ptr<Level>> levels_;
};

}  // namespace ghostdb
