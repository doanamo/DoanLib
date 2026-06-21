#include "dn/memory.h"

static DnMemArena g_dnMemArenaTemp = {};

typedef struct DnMemAllocatorTempHeader {
  u64 size;
} DnMemAllocatorTempHeader;

bool DnMemTemp_Init(const DnMemConfig* config) {
  u64 tempReservedSize = config->tempReservedSize ? config->tempReservedSize : DN_MEM_GB(1);
  DN_LOG_INFO("Reserved temporary memory: %.2f GB", DN_MEM_TO_GB(tempReservedSize));

  if (!DnMemArena_Init(&g_dnMemArenaTemp, tempReservedSize)) {
    return false;
  }

  return true;
}

void DnMemTemp_Deinit() {
  if (g_dnMemArenaTemp.usedSize != 0) {
    DN_LOG_ERROR("Leaked temporary memory: %.2f GB", DN_MEM_TO_GB(g_dnMemArenaTemp.usedSize));
  }

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

  if (scope->valid) {
    DN_ASSERT(g_dnMemArenaTemp.usedSize >= scope->savedUsedSize);
    g_dnMemArenaTemp.usedSize = scope->savedUsedSize;
    scope->valid = false;
  }
}

void* DnMemAllocatorTemp_Alloc(const DnMemAllocator* allocator, u64 size, u64 alignment) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);
  DN_ASSERT(size > 0);

  u64 alignmentPadding = alignment - 1;
  u64 alignedHeaderSize = DN_MEM_ALIGN_UP(sizeof(DnMemAllocatorTempHeader), alignment);
  void* allocation = DnMemArena_Push(&g_dnMemArenaTemp, alignmentPadding + alignedHeaderSize + size);
  DN_ASSERT_ALWAYS(allocation);

#if DN_MEM_PATTERNS_ENABLED
  memset(allocation, DnMem_PatternPadding, alignmentPadding + alignedHeaderSize);
  memset((u8*)allocation + size, DnMem_PatternPadding, alignmentPadding + alignedHeaderSize);
#endif

  allocation = (void*)(DN_MEM_ALIGN_UP((u64)allocation, alignment) + alignedHeaderSize);

  DnMemAllocatorTempHeader* header = (DnMemAllocatorTempHeader*)((u8*)allocation - sizeof(DnMemAllocatorTempHeader));
  header->size = size;

#if DN_MEM_PATTERNS_ENABLED
  memset(allocation, DnMem_PatternAllocated, size);
#endif

  return allocation;
}

void DnMemAllocatorTemp_Free(const DnMemAllocator* allocator, void* allocation) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);
  DN_UNUSED(allocation);

#if DN_MEM_PATTERNS_ENABLED
  if (allocation) {
    DnMemAllocatorTempHeader* header = (DnMemAllocatorTempHeader*)((u8*)allocation - sizeof(DnMemAllocatorTempHeader));
    memset(allocation, DnMem_PatternFreed, header->size);
  }
#endif
}

void* DnMemAllocatorTemp_Realloc(const DnMemAllocator* allocator, void* allocation, u64 size, u64 alignment) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  if (size == 0) {
    DnMemAllocatorTemp_Free(allocator, allocation);
    return nullptr;
  }

  // #todo: Optimize for resizing last allocation without creating a new block on top.

  void* reallocation = DnMemAllocatorTemp_Alloc(allocator, size, alignment);
  DN_ASSERT_ALWAYS(reallocation);

  if (allocation) {
    DnMemAllocatorTempHeader* header = (DnMemAllocatorTempHeader*)((u8*)allocation - sizeof(DnMemAllocatorTempHeader));
    memcpy(reallocation, allocation, header->size < size ? header->size : size);
    DnMemAllocatorTemp_Free(allocator, allocation);
  }

  return reallocation;
}

static DnMemAllocator g_dnMemAllocatorTempPrivate = {
  .alloc = DnMemAllocatorTemp_Alloc,
  .realloc = DnMemAllocatorTemp_Realloc,
  .free = DnMemAllocatorTemp_Free,
  .context = nullptr,
};

const DnMemAllocator* const g_dnMemAllocatorTemp = &g_dnMemAllocatorTempPrivate;
