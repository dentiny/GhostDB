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
  static bool Open(const char *path, GhostDB **db);
  bool Put(int32_t key, int32_t val);
  bool Get(int32_t key, int32_t *val);
  bool Delete(int32_t key);
  GhostDB();
  ~GhostDB() noexcept;

 private:
  std::unique_ptr<Buffer> buffer_;
  std::vector<Level*> levels_;
  std::unique_ptr<LogManager> log_manager_;
};

}  // namespace ghostdb
