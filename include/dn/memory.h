#pragma once

void* DnMem_Alloc(u64 size);
void* DnMem_Realloc(void* allocation, u64 size);
void DnMem_Free(void* allocation);

u64 DnMemVirtual_GetPageSize();
void* DnMemVirtual_Reserve(u64 size);
bool DnMemVirtual_Commit(void* ptr, u64 size);
void DnMemVirtual_Decommit(void* ptr, u64 size);
void DnMemVirtual_Release(void* ptr);

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

typedef struct DnMemArena {
  void* address;
  u64 reservedSize;
  u64 committedSize;
  u64 usedSize;
} DnMemArena;

bool DnMemArena_Init(DnMemArena* arena, u64 reserveSize);
void* DnMemArena_Alloc(DnMemArena* arena, u64 size);
void DnMemArena_Free(DnMemArena* arena, bool decommit);
void DnMemArena_Deinit(DnMemArena* arena);
