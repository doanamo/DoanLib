#pragma once

#include "shared.h"
#include "math.h"
#include <memory.h>

/*
 * Memory initialization
 */

typedef struct DnMemConfig {
  u64 tempReservedSize;
} DnMemConfig;

bool DnMem_Init(const DnMemConfig* config);
void DnMem_Deinit();

/*
 * Memory constants
 */

constexpr u64 DnMem_DefaultAlignment = alignof(max_align_t);
constexpr u64 DnMem_SystemPageSize = 4096;

constexpr u8 DnMem_PatternAllocated = 0xCD;
constexpr u8 DnMem_PatternFreed = 0xDD;
constexpr u8 DnMem_PatternPadding = 0xFD;

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
    u64 _size = (size); \
    u64 _alignment = (alignment); \
    DN_ASSERT(DN_IS_POW2(_alignment)); \
    ((_size) + (_alignment - 1)) & ~(_alignment - 1); \
  })

#define DN_MEM_KB(bytes) ((bytes) << 10)
#define DN_MEM_MB(bytes) ((bytes) << 20)
#define DN_MEM_GB(bytes) ((bytes) << 30)

#define DN_MEM_TO_KB(bytes) ((f64)(bytes) / (1ull << 10))
#define DN_MEM_TO_MB(bytes) ((f64)(bytes) / (1ull << 20))
#define DN_MEM_TO_GB(bytes) ((f64)(bytes) / (1ull << 30))

/*
 * Memory allocation
 */

#define DN_MEM_ALLOC(allocator, size) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->alloc(_allocator, size); \
  })

#define DN_MEM_REALLOC(allocator, allocation, size) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->realloc(_allocator, allocation, size); \
  })

#define DN_MEM_FREE(allocator, allocation) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->free(_allocator, allocation); \
  })

#define DN_MEM_ALLOC_TYPE(allocator, type) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    (type*)_allocator->alloc(_allocator, sizeof(type)); \
  })

#define DN_MEM_REALLOC_TYPES(allocator, allocation, type, count) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    (type*)_allocator->realloc(_allocator, allocation, sizeof(type) * (count)); \
  })

#define DN_MEM_ALLOC_ALIGNED(allocator, size, alignment) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->allocAligned(_allocator, size, alignment); \
  })

#define DN_MEM_REALLOC_ALIGNED(allocator, allocation, size, alignment) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->reallocAligned(_allocator, allocation, size, alignment); \
  })

#define DN_MEM_FREE_ALIGNED(allocator, allocation) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->freeAligned(_allocator, allocation); \
  })

#define DN_MEM_ALLOC_ALIGNED_TYPE(allocator, type) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    (type*)_allocator->allocAligned(_allocator, sizeof(type), alignof(type)); \
  })

#define DN_MEM_REALLOC_ALIGNED_TYPES(allocator, allocation, type, count) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    (type*)_allocator->reallocAligned(_allocator, allocation, sizeof(type) * (count), alignof(type)); \
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

// #todo: Add calloc

typedef struct DnMemAllocator DnMemAllocator;
typedef void* (DnMemAllocatorAllocFunc)(const DnMemAllocator* allocator, u64 size);
typedef void* (DnMemAllocatorReallocFunc)(const DnMemAllocator* allocator, void* pointer, u64 size);
typedef void (DnMemAllocatorFreeFunc)(const DnMemAllocator* allocator, void* pointer);
typedef void* (DnMemAllocatorAllocAlignedFunc)(const DnMemAllocator* allocator, u64 size, u64 alignment);
typedef void* (DnMemAllocatorReallocAlignedFunc)(const DnMemAllocator* allocator, void* pointer, u64 size, u64 alignment);
typedef void (DnMemAllocatorFreeAlignedFunc)(const DnMemAllocator* allocator, void* pointer);

typedef struct DnMemAllocator {
  DnMemAllocatorAllocFunc* alloc;
  DnMemAllocatorReallocFunc* realloc;
  DnMemAllocatorFreeFunc* free;
  DnMemAllocatorAllocAlignedFunc* allocAligned;
  DnMemAllocatorReallocAlignedFunc* reallocAligned;
  DnMemAllocatorFreeAlignedFunc* freeAligned;
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
