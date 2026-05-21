#include "dn/memory.h"
#include <stdlib.h>

void* DnMemAllocatorMalloc_Alloc(const DnMemAllocator* allocator, u64 size) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  void* allocated = malloc(size);
  DN_ASSERT_ALWAYS(allocated);

  return allocated;
}

void* DnMemAllocatorMalloc_Realloc(const DnMemAllocator* allocator, void* allocation, u64 oldSize, u64 newSize) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);
  DN_UNUSED(oldSize);

  void* reallocation = realloc(allocation, newSize);
  DN_ASSERT_ALWAYS(reallocation);

  return reallocation;
}

void DnMemAllocatorMalloc_Free(const DnMemAllocator* allocator, void* allocation, u64 size) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);
  DN_UNUSED(size);

  free(allocation);
}

void* DnMemAllocatorMalloc_AllocAligned(const DnMemAllocator* allocator, u64 size, u64 alignment) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  void* allocated = _aligned_malloc(size, alignment);
  DN_ASSERT_ALWAYS(allocated);

  return allocated;
}

void* DnMemAllocatorMalloc_ReallocAligned(const DnMemAllocator* allocator, void* allocation, u64 oldSize, u64 size, u64 alignment) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);
  DN_UNUSED(oldSize);

  void* reallocation = _aligned_realloc(allocation, size, alignment);
  DN_ASSERT_ALWAYS(reallocation);

  return reallocation;
}

void DnMemAllocatorMalloc_FreeAligned(const DnMemAllocator* allocator, void* allocation, u64 size) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);
  DN_UNUSED(size);

  _aligned_free(allocation);
}

static DnMemAllocator g_dnMemAllocatorMallocPrivate = {
  .alloc = &DnMemAllocatorMalloc_Alloc,
  .realloc = &DnMemAllocatorMalloc_Realloc,
  .free = &DnMemAllocatorMalloc_Free,
  .allocAligned = &DnMemAllocatorMalloc_AllocAligned,
  .reallocAligned = &DnMemAllocatorMalloc_ReallocAligned,
  .freeAligned = &DnMemAllocatorMalloc_FreeAligned,
  .context = nullptr,
};

const DnMemAllocator* const g_dnMemAllocatorDefault = &g_dnMemAllocatorMallocPrivate;
const DnMemAllocator* const g_dnMemAllocatorMalloc = &g_dnMemAllocatorMallocPrivate;
