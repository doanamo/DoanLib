#include "dn/memory.h"

// Temporary allocator intentionally leaks old allocations. There can be the worst case scenario for repeated reallocations (imagine continously resized array), but temporary allocator should be used for very short lived transient allocations only.

static DnMemArena g_dnMemArenaTemp = {};

void* DnMemAllocatorTemp_Alloc(const DnMemAllocator* allocator, u64 size) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  void* allocation = DnMemArena_Alloc(&g_dnMemArenaTemp, size);
  DN_ASSERT_ALWAYS(allocation);

  // #todo: Debug reset allocated memory to a specific pattern.
  return allocation;
}

void* DnMemAllocatorTemp_Realloc(const DnMemAllocator* allocator, void* allocation, u64 oldSize, u64 newSize) {
  void* reallocation = DnMemAllocatorTemp_Alloc(allocator, newSize);
  DN_ASSERT_ALWAYS(reallocation);

  if (allocation) {
    DN_ASSERT(oldSize);
    memcpy(reallocation, allocation, oldSize < newSize ? oldSize : newSize);
    // #todo: Debug reset old memory to a specific pattern.
  }

  return reallocation;
}

void DnMemAllocatorTemp_Free(const DnMemAllocator* allocator, void* allocation) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);
  DN_UNUSED(allocation);

  // #todo: Debug reset freed memory to a specific pattern.
}

void* DnMemAllocatorTemp_AllocAligned(const DnMemAllocator* allocator, u64 size, u64 alignment) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  u64 alignmentSize = alignment - 1;
  void* allocation = DnMemArena_Alloc(&g_dnMemArenaTemp, size + alignmentSize);
  DN_ASSERT_ALWAYS(allocation);

  // #todo: Debug reset allocated memory to a specific pattern.
  allocation = (void*)DN_MEM_ALIGN_UP((u64)allocation, alignment);
  return allocation;
}

void* DnMemAllocatorTemp_ReallocAligned(const DnMemAllocator* allocator, void* allocation, u64 oldSize, u64 newSize, u64 alignment) {
  void* reallocation = DnMemAllocatorTemp_AllocAligned(allocator, newSize, alignment);
  DN_ASSERT_ALWAYS(reallocation);

  if (allocation) {
    DN_ASSERT(oldSize);
    memcpy(reallocation, allocation, oldSize < newSize ? oldSize : newSize);
    // #todo: Debug reset old memory to a specific pattern.
  }

  return reallocation;
}

void DnMemAllocatorTemp_FreeAligned(const DnMemAllocator* allocator, void* allocation) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);
  DN_UNUSED(allocation);

  // #todo: Debug reset freed memory to a specific pattern.
}

bool DnMemTemp_Init(const DnMemConfig* config) {
  u64 tempReservedSize = config->tempReservedSize ? config->tempReservedSize : DN_MEM_GB(1);
  DN_LOG_INFO("Reserved temporary memory: %.2f GB", DN_MEM_TO_GB(tempReservedSize));

  if (!DnMemArena_Init(&g_dnMemArenaTemp, tempReservedSize)) {
    return false;
  }

  return true;
}

void DnMemTemp_Deinit() {
  DnMemArena_Deinit(&g_dnMemArenaTemp);
}

DnMemTempScope DnMemTemp_PushScope() {
  return (DnMemTempScope) {
    .savedUsedSize = g_dnMemArenaTemp.usedSize,
    .valid = true,
  };
}

void DnMemTemp_PopScope(DnMemTempScope* scope) {
  DN_ASSERT(scope);

  // Simplify scope handling by allowing pop to be called on invalid scopes.
  if (scope->valid) {
    DN_ASSERT(g_dnMemArenaTemp.usedSize >= scope->savedUsedSize);
    g_dnMemArenaTemp.usedSize = scope->savedUsedSize;
    scope->valid = false;
  }
}

static DnMemAllocator g_dnMemAllocatorTempPrivate = {
  .alloc = DnMemAllocatorTemp_Alloc,
  .realloc = DnMemAllocatorTemp_Realloc,
  .free = DnMemAllocatorTemp_Free,
  .allocAligned = DnMemAllocatorTemp_AllocAligned,
  .reallocAligned = DnMemAllocatorTemp_ReallocAligned,
  .freeAligned = DnMemAllocatorTemp_FreeAligned,
  .context = nullptr,
};

const DnMemAllocator* const g_dnMemAllocatorTemp = &g_dnMemAllocatorTempPrivate;
