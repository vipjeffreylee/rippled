//  Copyright (c) 2013, Facebook, Inc.  All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.
//
#include "../include/rocksdb/status.h"
#include "../include/rocksdb/env.h"

#include <vector>
#include "coding.h"
#include "testharness.h"

namespace rocksdb {

class LockTest {
 public:
  static LockTest* current_;
  std::string file_;
  rocksdb::Env* env_;

  LockTest() : file_(test::TmpDir() + "/db_testlock_file"),
               env_(rocksdb::Env::Default()) {
    current_ = this;
  }

  ~LockTest() {
  }

  Status LockFile(FileLock** db_lock) {
    return env_->LockFile(file_, db_lock);
  }

  Status UnlockFile(FileLock* db_lock) {
    return env_->UnlockFile(db_lock);
  }
};
LockTest* LockTest::current_;

TEST(LockTest, LockBySameThread) {
  FileLock* lock1;
  FileLock* lock2;

  // acquire a lock on a file
  ASSERT_OK(LockFile(&lock1));

  // re-acquire the lock on the same file. This should fail.
  ASSERT_TRUE(LockFile(&lock2).IsIOError());

  // release the lock
  ASSERT_OK(UnlockFile(lock1));

}

}  // namespace rocksdb

int main(int argc, char** argv) {
  return rocksdb::test::RunAllTests();
}
