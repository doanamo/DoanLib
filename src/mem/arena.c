#include "dn/shared.h"
#include "dn/mem/arena.h"
#include "dn/mem/virtual.h"

bool DnMemArenaInit(DnMemArena* arena, u64 reserveSize) {
  DN_ASSERT(arena != nullptr);
  DN_ASSERT(reserveSize > 0);

  reserveSize = DN_ALIGN_UP(reserveSize, DnMemVirtualPageSize());
  void* address = DnMemVirtualReserve(reserveSize);
  if (address == nullptr) {
    return false;
  }

  *arena = (DnMemArena){
    .address = address,
    .reservedSize = reserveSize,
    .committedSize = 0,
    .usedSize = 0,
  };

  return true;
}

void* DnMemArenaAlloc(DnMemArena* arena, u64 size) {
  DN_ASSERT(arena != nullptr);
  DN_ASSERT(size > 0);

  u64 allocationOffset = DN_ALIGN_UP(arena->usedSize, DN_DEFAULT_ALIGNMENT);

  u64 newUsedSize = allocationOffset + size;
  if (newUsedSize > arena->reservedSize) {
    return nullptr;
  }

  if (newUsedSize > arena->committedSize) {
    u64 newCommittedSize = DN_ALIGN_UP(newUsedSize, DnMemVirtualPageSize());

    void* pageCommitAddress = arena->address + arena->committedSize;
    u64 pageCommitSize = newCommittedSize - arena->committedSize;
    if (!DnMemVirtualCommit(pageCommitAddress, pageCommitSize)) {
      return nullptr;
    }

    arena->committedSize = newCommittedSize;
  }

  arena->usedSize = newUsedSize;
  return arena->address + allocationOffset;
}

void DnMemArenaFree(DnMemArena* arena, bool decommit) {
  DN_ASSERT(arena != nullptr);
  arena->usedSize = 0;

  if (decommit) {
    DnMemVirtualDecommit(arena->address, arena->committedSize);
    arena->committedSize = 0;
  }
}

void DnMemArenaDeinit(DnMemArena* arena) {
  DN_ASSERT(arena != nullptr);
  DnMemVirtualRelease(arena->address);
  *arena = (DnMemArena){};
}
