#pragma once

typedef struct DnMemAllocator DnMemAllocator;

typedef void* (DnMemAlloc)(DnMemAllocator* allocator, u64 size);
typedef void* (DnMemRealloc)(DnMemAllocator* context, void* ptr, u64 size);
typedef void (DnMemFree)(DnMemAllocator* context, void* ptr);

typedef struct DnMemAllocator {
  DnMemAlloc* alloc;
  DnMemRealloc* realloc;
  DnMemFree* free;
  void* context;
} DnMemAllocator;

extern DnMemAllocator g_dnMemAllocatorMalloc;

bool DnMemAllocatorsInit();
void DnMemAllocatorsDeinit();
