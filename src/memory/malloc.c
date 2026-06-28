#include "dn/memory.h"
#include <malloc.h>

void* DnMemAllocatorMalloc_Alloc(const DnMemAllocator* allocator, u64 size, u64 alignment) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  void* allocated = _aligned_malloc(size, alignment);
  DN_ASSERT_ALWAYS(allocated);

  return allocated;
}

void* DnMemAllocatorMalloc_Realloc(const DnMemAllocator* allocator, void* allocation, u64 size, u64 alignment) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  void* reallocation = _aligned_realloc(allocation, size, alignment);
  DN_ASSERT_ALWAYS(reallocation);

  return reallocation;
}

void DnMemAllocatorMalloc_Free(const DnMemAllocator* allocator, void* allocation) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  _aligned_free(allocation);
}

static DnMemAllocator g_dnMemAllocatorMallocPrivate = {
  .alloc = &DnMemAllocatorMalloc_Alloc,
  .realloc = &DnMemAllocatorMalloc_Realloc,
  .free = &DnMemAllocatorMalloc_Free,
  .context = nullptr,
};

// #todo: Implement custom general purpose allocator instead of using malloc().
const DnMemAllocator* g_dnMemAllocatorDefault = &g_dnMemAllocatorMallocPrivate;
const DnMemAllocator* g_dnMemAllocatorMalloc = &g_dnMemAllocatorMallocPrivate;
