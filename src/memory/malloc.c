#include "dn/memory.h"
#include <malloc.h>

// == MALLOC ALLOCATOR ====================================================== //

static void* DnMemAllocatorMalloc_Alloc(const DnMemAllocator* allocator, u64 size, u64 alignment) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  void* allocated = _aligned_malloc(size, alignment);
  DN_ASSERT_ALWAYS(allocated);

  return allocated;
}

static void* DnMemAllocatorMalloc_Realloc(const DnMemAllocator* allocator, void* allocation, u64 size, u64 alignment) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  void* reallocation = _aligned_realloc(allocation, size, alignment);
  DN_ASSERT_ALWAYS(reallocation);

  return reallocation;
}

static void DnMemAllocatorMalloc_Free(const DnMemAllocator* allocator, void* allocation) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  _aligned_free(allocation);
}

static DnMemAllocator g_dnMemAllocatorMalloc = {
  .alloc = &DnMemAllocatorMalloc_Alloc,
  .realloc = &DnMemAllocatorMalloc_Realloc,
  .free = &DnMemAllocatorMalloc_Free,
  .context = nullptr,
};

const DnMemAllocator* DnMemMalloc_GetAllocator() {
  return &g_dnMemAllocatorMalloc;
}
