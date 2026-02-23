#pragma once

typedef struct DnMemAllocator DnMemAllocator;

typedef void* (DnMemAllocFunc)(DnMemAllocator* allocator, u64 size);
typedef void* (DnMemReallocFunc)(DnMemAllocator* allocator, void* pointer, u64 size);
typedef void (DnMemFreeFunc)(DnMemAllocator* allocator, void* pointer);

typedef struct DnMemAllocator {
  DnMemAllocFunc* alloc;
  DnMemReallocFunc* realloc;
  DnMemFreeFunc* free;
  void* context;
} DnMemAllocator;

bool DnMemAllocators_Init();
void DnMemAllocators_Deinit();

const DnMemAllocator* DnMemAllocatorMalloc_Get();
