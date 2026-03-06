#pragma once

bool DnMem_Init();
void DnMem_Deinit();

/*
 * Memory constants
 */

constexpr u64 DnMem_DefaultAlignment = alignof(max_align_t);
constexpr u64 DnMem_SystemPageSize = 4096;

/*
 * Memory macros
 */

#define DN_MEM_ALIGN_UP(size, alignment) \
  ({ \
    u64 _size = (size); \
    u64 _alignment = (alignment); \
    DN_ASSERT(DN_IS_POW2(_alignment)); \
    (_size + (_alignment - 1)) & ~(_alignment - 1); \
  })

#define DN_MEM_KB(size) \
  ({ \
    u64 _size = (size); \
    _size << 10; \
  })

#define DN_MEM_MB(size) \
  ({ \
    u64 _size = (size); \
    _size << 20; \
  })

#define DN_MEM_GB(size) \
  ({ \
    u64 _size = (size); \
    _size << 30; \
  })

/*
 * Memory allocation
 */

void* DnMem_Alloc(u64 size);
void* DnMem_Realloc(void* allocation, u64 size);
void DnMem_Free(void* allocation);

#define DN_MEM_ALLOC(type) \
  ({ \
    (type*)DnMem_Alloc(sizeof(type)); \
  })

#define DN_MEM_REALLOC(allocation, type, count) \
  ({ \
    void* _allocation = (allocation); \
    u64 _size = sizeof(type) * (count); \
    (type*)DnMem_Realloc(_allocation, _size); \
  })

#define DN_MEM_FREE(allocation) \
  ({ \
    void* _allocation = (allocation); \
    DnMem_Free(_allocation); \
  })

/*
 * Virtual memory
 */

void* DnMemVirtual_Reserve(u64 size);
bool DnMemVirtual_Commit(void* page, u64 size);
void DnMemVirtual_Decommit(void* page, u64 size);
void DnMemVirtual_Release(void* page);

/*
 * Memory allocator
 */

typedef struct DnMemAllocator DnMemAllocator;
typedef void* (DnMemAllocatorAllocFunc)(const DnMemAllocator* allocator, u64 size);
typedef void* (DnMemAllocatorReallocFunc)(const DnMemAllocator* allocator, void* pointer, u64 size);
typedef void (DnMemAllocatorFreeFunc)(const DnMemAllocator* allocator, void* pointer);

typedef struct DnMemAllocator {
  DnMemAllocatorAllocFunc* alloc;
  DnMemAllocatorReallocFunc* realloc;
  DnMemAllocatorFreeFunc* free;
  void* context;
} DnMemAllocator;

const DnMemAllocator* DnMemAllocatorMalloc_Get();

#define DN_MEM_ALLOCATOR_ALLOC(allocator, type) \
  ({ \
    const DnMemAllocator* _allocator = (allocator); \
    (type*)_allocator->alloc(_allocator, sizeof(type)); \
  })

#define DN_MEM_ALLOCATOR_REALLOC(allocator, allocation, type, count) \
  ({ \
    const DnMemAllocator* _allocator = (allocator); \
    (type*)_allocator->realloc(_allocator, allocation, sizeof(type) * (count)); \
  })

#define DN_MEM_ALLOCATOR_FREE(allocator, allocation) \
  ({ \
    const DnMemAllocator* _allocator = (allocator); \
    _allocator->free(_allocator, allocation); \
  })

/*
 * Memory arena
 */

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
