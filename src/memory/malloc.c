#include "dn/memory.h"
#include <malloc.h>

// == MALLOC ALLOCATOR ====================================================== //

static void* DnMemMalloc_Alloc(const DnMemAllocator* allocator, u64 size, u64 alignment) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  void* allocated = _aligned_malloc(size, alignment);
  DN_ASSERT_ALWAYS(allocated);

  return allocated;
}

static void* DnMemMalloc_Realloc(const DnMemAllocator* allocator, void* allocation, u64 size, u64 alignment) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  void* reallocation = _aligned_realloc(allocation, size, alignment);
  DN_ASSERT_ALWAYS(reallocation);

  return reallocation;
}

static void DnMemMalloc_Free(const DnMemAllocator* allocator, void* allocation) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  _aligned_free(allocation);
}

static DnMemAllocator g_dnMemMallocAllocator = {
  .alloc = &DnMemMalloc_Alloc,
  .realloc = &DnMemMalloc_Realloc,
  .free = &DnMemMalloc_Free,
  .context = nullptr,
};

const DnMemAllocator* DnMemMalloc_GetAllocator() {
  return &g_dnMemMallocAllocator;
}
