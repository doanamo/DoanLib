#include "dn/shared.h"
#include "dn/mem/allocators.h"

DnMemAllocator g_dnMemAllocatorMalloc = {};

void* DnMalloc(DnMemAllocator* allocator, size_t size) {
  DN_UNUSED(allocator);
  return malloc(size);
}

void* DnRealloc(DnMemAllocator* allocator, void* pointer, size_t size) {
  DN_UNUSED(allocator);
  return realloc(pointer, size);
}

void DnFree(DnMemAllocator* allocator, void* pointer) {
  DN_UNUSED(allocator);
  free(pointer);
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
