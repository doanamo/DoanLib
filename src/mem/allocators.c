#include "dn/shared.h"
#include "dn/mem/allocators.h"

DnMemAllocator g_dnMemAllocatorMalloc = {};

void* DnMalloc(DnMemAllocator* allocator, u64 size) {
  DN_UNUSED(allocator);
  return malloc(size);
}

void* DnRealloc(DnMemAllocator* allocator, void* ptr, u64 size) {
  DN_UNUSED(allocator);
  return realloc(ptr, size);
}

void DnFree(DnMemAllocator* allocator, void* ptr) {
  DN_UNUSED(allocator);
  free(ptr);
}

bool DnMemAllocatorsInit() {
  DN_LOG_INFO("Initializing memory allocators");

  g_dnMemAllocatorMalloc = (DnMemAllocator){
    .alloc = &DnMalloc,
    .realloc = &DnRealloc,
    .free = &DnFree,
    .context = nullptr,
  };

  return true;
}

void DnMemAllocatorsDeinit() {
  DN_LOG_INFO("Deinitializing memory allocators");
}
