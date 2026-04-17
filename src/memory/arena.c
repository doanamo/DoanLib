#include "dn/memory.h"

bool DnMemArena_Init(DnMemArena* arena, u64 reserveSize) {
  DN_ASSERT(arena != nullptr);
  DN_ASSERT(reserveSize > 0);

  reserveSize = DN_MEM_ALIGN_UP(reserveSize, DnMem_SystemPageSize);
  void* address = DnMemVirtual_Reserve(reserveSize);
  if (address == nullptr) {
    DN_LOG_ERROR("Failed to reserve memory for arena");
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

void* DnMemArena_Alloc(DnMemArena* arena, u64 allocationSize) {
  DN_ASSERT(arena != nullptr);
  DN_ASSERT(allocationSize > 0);

  DN_ASSERT(DN_MEM_IS_ALIGNED(arena->usedSize, DnMem_DefaultAlignment));
  allocationSize = DN_MEM_ALIGN_UP(allocationSize, DnMem_DefaultAlignment);

  u64 newUsedSize = arena->usedSize + allocationSize;
  if (newUsedSize > arena->reservedSize) {
    DN_LOG_ERROR("Allocation exceeds reserved arena memory");
    return nullptr;
  }

  if (newUsedSize > arena->committedSize) {
    u64 newCommittedSize = DN_MEM_ALIGN_UP(newUsedSize, DnMem_SystemPageSize);

    void* pageCommitAddress = arena->address + arena->committedSize;
    u64 pageCommitSize = newCommittedSize - arena->committedSize;
    if (!DnMemVirtual_Commit(pageCommitAddress, pageCommitSize)) {
      DN_LOG_ERROR("Failed to commit memory for arena");
      return nullptr;
    }

    arena->committedSize = newCommittedSize;
  }

  void* allocation = arena->address + arena->usedSize;
  arena->usedSize = newUsedSize;
  return allocation;
}

void DnMemArena_Free(DnMemArena* arena, u64 allocationSize) {
  DN_ASSERT(arena != nullptr);
  DN_ASSERT(allocationSize > 0);

  DN_ASSERT(DN_MEM_IS_ALIGNED(arena->usedSize, DnMem_DefaultAlignment));
  allocationSize = DN_MEM_ALIGN_UP(allocationSize, DnMem_DefaultAlignment);

  DN_ASSERT(arena->usedSize >= allocationSize);
  arena->usedSize = arena->usedSize - allocationSize;
}

void DnMemArena_Reset(DnMemArena* arena, bool decommit) {
  DN_ASSERT(arena != nullptr);
  arena->usedSize = 0;

  if (decommit) {
    DnMemVirtual_Decommit(arena->address, arena->committedSize);
    arena->committedSize = 0;
  }
}

void DnMemArena_Deinit(DnMemArena* arena) {
  DN_ASSERT(arena != nullptr);

  if (arena->address)
  {
    DnMemVirtual_Release(arena->address);
  }

  *arena = (DnMemArena){};
}
