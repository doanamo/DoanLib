#include "dn/memory.h"

bool DnMemArena_Init(DnMemArena* arena, u64 reserveSize) {
  DN_ASSERT(arena != nullptr);
  DN_ASSERT(reserveSize > 0);

  reserveSize = DN_ALIGN_UP(reserveSize, DnMemVirtual_GetPageSize());
  void* address = DnMemVirtual_Reserve(reserveSize);
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

void* DnMemArena_Alloc(DnMemArena* arena, u64 size) {
  DN_ASSERT(arena != nullptr);
  DN_ASSERT(size > 0);

  u64 allocationOffset = DN_ALIGN_UP(arena->usedSize, DN_DEFAULT_ALIGNMENT);

  u64 newUsedSize = allocationOffset + size;
  if (newUsedSize > arena->reservedSize) {
    return nullptr;
  }

  if (newUsedSize > arena->committedSize) {
    u64 newCommittedSize = DN_ALIGN_UP(newUsedSize, DnMemVirtual_GetPageSize());

    void* pageCommitAddress = arena->address + arena->committedSize;
    u64 pageCommitSize = newCommittedSize - arena->committedSize;
    if (!DnMemVirtual_Commit(pageCommitAddress, pageCommitSize)) {
      return nullptr;
    }

    arena->committedSize = newCommittedSize;
  }

  arena->usedSize = newUsedSize;
  return arena->address + allocationOffset;
}

void DnMemArena_Free(DnMemArena* arena, bool decommit) {
  DN_ASSERT(arena != nullptr);
  arena->usedSize = 0;

  if (decommit) {
    DnMemVirtual_Decommit(arena->address, arena->committedSize);
    arena->committedSize = 0;
  }
}

void DnMemArena_Deinit(DnMemArena* arena) {
  DN_ASSERT(arena != nullptr);
  DnMemVirtual_Release(arena->address);
  *arena = (DnMemArena){};
}
