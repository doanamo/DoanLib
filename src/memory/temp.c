#include "dn/memory.h"

static DnMemArena g_dnMemArenaTemp = {};

void* DnMemAllocatorTemp_Alloc(const DnMemAllocator* allocator, u64 size) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  void* allocation = DnMemArena_Alloc(&g_dnMemArenaTemp, size);
  DN_ASSERT_ALWAYS(allocation);

  // #todo: Debug reset allocated memory to a specific pattern.
  return allocation;
}

// Temporary allocator intentionally leaks old allocations. There can be the worst case scenario for repeated reallocations (imagine continously resized array), but temporary allocator should be used for very short lived transient allocations only.
void* DnMemAllocatorTemp_Realloc(const DnMemAllocator* allocator, void* allocation, u64 oldSize, u64 newSize) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  void* reallocation = DnMemArena_Alloc(&g_dnMemArenaTemp, newSize);
  DN_ASSERT_ALWAYS(reallocation);

  if (allocation)
  {
    DN_ASSERT(oldSize);
    memcpy(reallocation, allocation, oldSize < newSize ? oldSize : newSize);
    // #todo: Debug reset old memory to a specific pattern.
  }

  return reallocation;
}

// Temporary allocator does not free allocations.
void DnMemAllocatorTemp_Free(const DnMemAllocator* allocator, void* allocation) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);
  DN_UNUSED(allocation);

  // #todo: Debug reset freed memory to a specific pattern.
}

bool DnMemTemp_Init(const DnMemConfig* config) {
  u64 tempReservedSize = config->tempReservedSize ? config->tempReservedSize : DN_MEM_GB(1);
  DN_LOG_INFO("Reserved temporary memory size: %.2f GB", DN_MEM_TO_GB(tempReservedSize));

  if (!DnMemArena_Init(&g_dnMemArenaTemp, tempReservedSize)) {
    return false;
  }

  return true;
}

void DnMemTemp_Deinit() {
  DnMemArena_Deinit(&g_dnMemArenaTemp);
}

DnMemTempScope DnMemTemp_PushScope() {
  return (DnMemTempScope){
    .savedUsedSize = g_dnMemArenaTemp.usedSize,
    .valid = true,
  };
}

void DnMemTemp_PopScope(DnMemTempScope* scope) {
  DN_ASSERT(scope);

  // Simplify scope handling by allowing pop to be called on invalid scopes.
  if (scope->valid)
  {
    DN_ASSERT(g_dnMemArenaTemp.usedSize >= scope->savedUsedSize);
    g_dnMemArenaTemp.usedSize = scope->savedUsedSize;
  }

  scope->valid = false;
}

static DnMemAllocator g_dnMemAllocatorTempPrivate = {
  .alloc = DnMemAllocatorTemp_Alloc,
  .realloc = DnMemAllocatorTemp_Realloc,
  .free = DnMemAllocatorTemp_Free,
  .context = nullptr,
};

const DnMemAllocator* const g_dnMemAllocatorTemp = &g_dnMemAllocatorTempPrivate;
