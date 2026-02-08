#pragma once

typedef struct DnMemAllocator DnMemAllocator;

typedef void* (DnMemAlloc)(DnMemAllocator* allocator, size_t size);
typedef void* (DnMemRealloc)(DnMemAllocator* context, void* pointer, size_t size);
typedef void (DnMemFree)(DnMemAllocator* context, void* pointer);

typedef struct DnMemAllocator {
  DnMemAlloc* alloc;
  DnMemRealloc* realloc;
  DnMemFree* free;
  void* context;
} DnMemAllocator;

extern DnMemAllocator g_dnMemAllocatorMalloc;

bool DnMemAllocatorsInit();
void DnMemAllocatorsDeinit();
