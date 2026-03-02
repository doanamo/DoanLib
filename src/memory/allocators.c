#include "dn/memory.h"

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

static DnMemAllocator g_dnMemAllocatorMalloc = {};
const DnMemAllocator* DnMemAllocatorMalloc_Get() {
  return &g_dnMemAllocatorMalloc;
}

bool DnMemAllocators_Init() {
  DN_LOG_INFO("Initializing memory allocators");

  g_dnMemAllocatorMalloc = (DnMemAllocator){
    .alloc = &DnMemAllocatorMalloc_Alloc,
    .realloc = &DnMemAllocatorMalloc_Realloc,
    .free = &DnMemAllocatorMalloc_Free,
    .context = nullptr,
  };

  return true;
}

void DnMemAllocators_Deinit() {
  DN_LOG_INFO("Deinitializing memory allocators");
  g_dnMemAllocatorMalloc = (DnMemAllocator){};
}
