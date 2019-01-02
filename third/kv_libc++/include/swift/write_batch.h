// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// WriteBatch holds a collection of updates to apply atomically to a DB.
//
// The updates are applied in the order in which they are added
// to the WriteBatch.  For example, the value of "key" will be "v3"
// after the following batch is written:
//
//    batch.Put("key", "v1");
//    batch.Delete("key");
//    batch.Put("key", "v2");
//    batch.Put("key", "v3");
//
// Multiple threads can invoke const methods on a WriteBatch without
// external synchronization, but if any of the threads may call a
// non-const method, all threads accessing the same WriteBatch must use
// external synchronization.

#ifndef SHANNON_DB_INCLUDE_WRITE_BATCH_H_
#define SHANNON_DB_INCLUDE_WRITE_BATCH_H_

#include <string>
#include <list>
#include "status.h"

namespace shannon {

class Slice;
class ColumnFamilyHandle;

class WriteBatch {
 public:
  WriteBatch();
  ~WriteBatch();

  void Put(ColumnFamilyHandle* column_family, const Slice& key,
          const Slice& value);
  // Store the mapping "key->value" in the database.
  void Put(const Slice& key, const Slice& value);

  void Delete(ColumnFamilyHandle* column_family, const Slice& key);
  // If the database contains a mapping for "key", erase it.  Else do nothing.
  void Delete(const Slice& key);

  // Clear all updates buffered in this batch.
  void Clear();

  // Support for iterating over the contents of a batch.
  class Handler {
   public:
    virtual ~Handler();
    virtual void Put(const Slice& key, const Slice& value) = 0;
    virtual void Delete(const Slice& key) = 0;
  };
  Status Iterate(Handler* handler) const;

 private:
  friend class WriteBatchInternal;

  std::string rep_;  // See comment in write_batch.cc for the format of rep_
  std::list<std::string> value_;

  // Intentionally copyable
};

}  // namespace shannon

#endif  // SHANNON_DB_INCLUDE_WRITE_BATCH_H_