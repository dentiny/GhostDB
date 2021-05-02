# GhostDB

GhostDB is a key-value store which supports ``get``, ``put``, ``delete``, ``range`` and ``query``.
It also supports configuration update at ``config.h``.

Example use:
```
// Header files needed for GhostDB.
#include "common.h"
#include "db.h"

// Create a GhostDB instance.
unique_ptr<GhostDB> db = make_unique<GhostDB>();

// Place key-value pair.
Key val = 0;
Val val = 0;
db->Put(key, val);

// Delete a key-value pair.
db->Delete(0 /* key */);

// Get corresponding pair.
Key k = 10;
Val v;
if (db->Get(k, &v)) {
  // key-value pair exists
}

// Get key-value pairs within range, inclusively.
buffer_t res;
db->GetRange(30 /* key1 */, 59 /* key2 */, &res);
```

### Checklist

- [ ] Update build system to CMakeLists and Bazel.
- [ ] buffer pool manager
  + NewPage, DeallocatePage, PinPage, UnpinPage
  + Allow new page allocated if currently SSTable page exhausted
  + RecoveryManager load multiple WAL and temporary SSTable pages
- [ ] concurrent access to GhostDB
- [ ] more thorough unit tests
