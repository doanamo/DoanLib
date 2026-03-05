#include "dn/memory.h"
#include <stdlib.h>

void* DnMem_Alloc(u64 size) {
  void* allocated = malloc(size);
  DN_ASSERT_ALWAYS(allocated);
  return allocated;
}

void* DnMem_Realloc(void* allocation, u64 size) {
  void* reallocation = realloc(allocation, size);
  DN_ASSERT_ALWAYS(reallocation);
  return reallocation;
}

void DnMem_Free(void* allocation) {
  free(allocation);
}

void* DnMemAllocatorMalloc_Alloc(const DnMemAllocator* allocator, u64 size) {
  DN_UNUSED(allocator);
  DN_ASSERT(allocator);
  return DnMem_Alloc(size);
}

void* DnMemAllocatorMalloc_Realloc(const DnMemAllocator* allocator, void* allocation, u64 size) {
  DN_UNUSED(allocator);
  DN_ASSERT(allocator);
  return DnMem_Realloc(allocation, size);
}

void DnMemAllocatorMalloc_Free(const DnMemAllocator* allocator, void* allocation) {
  DN_UNUSED(allocator);
  DN_ASSERT(allocator);
  DnMem_Free(allocation);
}

static DnMemAllocator g_dnMemAllocatorMalloc = {
  .alloc = &DnMemAllocatorMalloc_Alloc,
  .realloc = &DnMemAllocatorMalloc_Realloc,
  .free = &DnMemAllocatorMalloc_Free,
  .context = nullptr,
};

const DnMemAllocator* DnMemAllocatorMalloc_Get() {
  return &g_dnMemAllocatorMalloc;
}
