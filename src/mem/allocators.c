#include "dn/mem/allocators.h"
#include "dn/mem/malloc.h"

static DnMemAllocator g_dnMemAllocatorMalloc = {};
const DnMemAllocator* DnMemAllocatorMalloc_Get() {
  return &g_dnMemAllocatorMalloc;
}

void* DnMemAllocatorMalloc_Alloc(DnMemAllocator* allocator, u64 size) {
  DN_UNUSED(allocator);
  DN_ASSERT(allocator);
  return DnMem_Alloc(size);
}

void* DnMemAllocatorMalloc_Realloc(DnMemAllocator* allocator, void* ptr, u64 size) {
  DN_UNUSED(allocator);
  DN_ASSERT(allocator);
  return DnMem_Realloc(ptr, size);
}

void DnMemAllocatorMalloc_Free(DnMemAllocator* allocator, void* ptr) {
  DN_UNUSED(allocator);
  DN_ASSERT(allocator);
  DnMem_Free(ptr);
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
}
