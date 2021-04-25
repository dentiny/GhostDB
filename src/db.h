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
#include "config.h"
#include "level.h"
#include "logger.h"
#include "log_manager.h"

namespace ghostdb {

class GhostDB {
 public:
  GhostDB();
  ~GhostDB() noexcept;
  bool Put(int32_t key, int32_t val);
  bool Get(int32_t key, int32_t *val);
  bool Delete(int32_t key);

 private:
  /** DB holds exactly one currently-using buffer */
  std::unique_ptr<Buffer> buffer_;
  /** DB holds at most MAX_LEVEL_NUM levels, each has upper limit of runs */
  std::vector<std::unique_ptr<Level*>> levels_;
};

}  // namespace ghostdb
