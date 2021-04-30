//===----------------------------------------------------------------------===//
//
//                         GhostDB
//
// recovery_manager.cc
//
// Identification: src/recovery_manager.cc
//
// Copyright (c) 2021
//
//===----------------------------------------------------------------------===//

#include "logger.h"
#include "page.h"
#include "recovery_manager.h"
#include "sstable_page.h"
#include "util.h"
namespace ghostdb {

RecoveryManager::RecoveryManager(DiskManager *disk_manager) :
  disk_manager_(disk_manager) {}

void RecoveryManager::PerformRecover() {
  RecoverSSTable();
  RecoverWAL();
}

// TODO: currently cannot 
void RecoveryManager::RecoverSSTable() {
  SSTablePage sstable_page;
  disk_manager_->ReadDb(sstable_page.data_);
  if (sstable_page.ParsePageData()) {
    LOG_DEBUG("temporary SSTable file is empty, no need to recover SSTable");
    return;
  }
  LOG_DEBUG("need to recovery SSTable file");
  int page_id = sstable_page.GetPageId();
  disk_manager_->WriteDb(sstable_page.data_, page_id);
}

// WAL is guarenteed to be within PAGE_SIZE.
void RecoveryManager::RecoverWAL() {
  char page_data[PAGE_SIZE] = { 0 };
  disk_manager_->ReadLog(page_data, PAGE_SIZE);
  if (IsPageAllZero(page_data)) {
    LOG_DEBUG("WAL is empty, no need to recover buffer");
    return;
  }

  // Keep reading key-value pairs, until two <0, 0> is met.
  // LOG_DEBUG("need to recover WAL file");
  // bool has_met_zero = false;
  // size_t offset = 0;
  // Key key;
  // Val val;
  // while (true) {
  //   memmove(&key, page_data, sizeof(key));
  //   offset += sizeof(key);
  //   memmove(&val, page_data, sizeof(val));
  //   offset += sizeof(val);

  //   // TODO
  // }
}

}  // namespace ghostdb
