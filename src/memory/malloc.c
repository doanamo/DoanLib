#include "dn/memory.h"
#include <stdlib.h>

void* DnMemAllocatorMalloc_Alloc(const DnMemAllocator* allocator, u64 size) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  void* allocated = malloc(size);
  DN_ASSERT_ALWAYS(allocated);
  return allocated;
}

void* DnMemAllocatorMalloc_Realloc(const DnMemAllocator* allocator, void* allocation, u64 size) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  void* reallocation = realloc(allocation, size);
  DN_ASSERT_ALWAYS(reallocation);
  return reallocation;
}

void DnMemAllocatorMalloc_Free(const DnMemAllocator* allocator, void* allocation) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);
  free(allocation);
}

DnMemAllocator g_dnMemAllocatorMalloc = {
  .alloc = &DnMemAllocatorMalloc_Alloc,
  .realloc = &DnMemAllocatorMalloc_Realloc,
  .free = &DnMemAllocatorMalloc_Free,
  .context = nullptr,
};

const DnMemAllocator* DnMemAllocatorMalloc_Get() {
  return &g_dnMemAllocatorMalloc;
}
