#pragma once

bool DnMem_Init();
void DnMem_Deinit();

/*
 * Memory constants
 */

constexpr u64 DnMem_DefaultAlignment = alignof(max_align_t);
constexpr u64 DnMem_SystemPageSize = 4096;

/*
 * Memory utility
 */

#define DN_MEM_IS_ALIGNED(size, alignment) \
  ({ \
    u64 _size = (size); \
    u64 _alignment = (alignment); \
    DN_ASSERT(DN_IS_POW2(_alignment)); \
    (_size & (_alignment - 1)) == 0; \
  })

#define DN_MEM_ALIGN_UP(size, alignment) \
  ({ \
    u64 _alignment = (alignment); \
    DN_ASSERT(DN_IS_POW2(_alignment)); \
    ((size) + (_alignment - 1)) & ~(_alignment - 1); \
  })

#define DN_MEM_KB(size) ((size) << 10)
#define DN_MEM_MB(size) ((size) << 20)
#define DN_MEM_GB(size) ((size) << 30)

/*
 * Memory allocation
 */

#define DN_MEM_ALLOC(allocator, size) \
  ({ \
    const DnMemAllocator* _allocator = (allocator); \
    _allocator->alloc(_allocator, (size)); \
  })

#define DN_MEM_REALLOC(allocator, allocation, oldSize, newSize) \
  ({ \
    const DnMemAllocator* _allocator = (allocator); \
    _allocator->realloc(_allocator, allocation, (oldSize), (newSize)); \
  })

#define DN_MEM_ALLOC_TYPE(allocator, type) \
  ({ \
    const DnMemAllocator* _allocator = (allocator); \
    (type*)_allocator->alloc(_allocator, sizeof(type)); \
  })

#define DN_MEM_REALLOC_TYPE(allocator, allocation, type, oldCount, newCount) \
  ({ \
    const DnMemAllocator* _allocator = (allocator); \
    (type*)_allocator->realloc(_allocator, allocation, sizeof(type) * (oldCount), sizeof(type) * (newCount)); \
  })

#define DN_MEM_FREE(allocator, allocation) \
  ({ \
    const DnMemAllocator* _allocator = (allocator); \
    _allocator->free(_allocator, allocation); \
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
typedef void* (DnMemAllocatorReallocFunc)(const DnMemAllocator* allocator, void* pointer, u64 oldSize, u64 newSize);
typedef void (DnMemAllocatorFreeFunc)(const DnMemAllocator* allocator, void* pointer);

typedef struct DnMemAllocator {
  DnMemAllocatorAllocFunc* alloc;
  DnMemAllocatorReallocFunc* realloc;
  DnMemAllocatorFreeFunc* free;
  void* context;
} DnMemAllocator;

extern const DnMemAllocator* const g_dnMemAllocatorDefault;
extern const DnMemAllocator* const g_dnMemAllocatorMalloc;

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
void* DnMemArena_Alloc(DnMemArena* arena, u64 allocationSize);
void DnMemArena_Free(DnMemArena* arena, u64 allocationSize);
void DnMemArena_Reset(DnMemArena* arena, bool decommit);
void DnMemArena_Deinit(DnMemArena* arena);

/*
 * Temporary memory
 */

typedef struct DnMemTempScope {
  u64 savedUsedSize;
  bool valid;
} DnMemTempScope;

DnMemTempScope DnMemTemp_PushScope();
void DnMemTemp_PopScope(DnMemTempScope* scope);

extern const DnMemAllocator* const g_dnMemAllocatorTemp;
