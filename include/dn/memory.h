#pragma once

/*
 * Memory macros
 */

#define DN_DEFAULT_ALIGNMENT alignof(max_align_t)

#define DN_ALIGN_UP(size, alignment) \
  ({ \
    u64 _size = (size); \
    u64 _alignment = (alignment); \
    DN_ASSERT(DN_IS_POW2(_alignment)); \
    (_size + (_alignment - 1)) & ~(_alignment - 1); \
  })

#define DN_KiB(size) \
  ({ \
    u64 _size = (size); \
    _size << 10; \
  })

#define DN_MiB(size) \
  ({ \
    u64 _size = (size); \
    _size << 20; \
  })

#define DN_GiB(size) \
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

/*
 * Virtual memory
 */

u64 DnMemVirtual_GetPageSize();
void* DnMemVirtual_Reserve(u64 size);
bool DnMemVirtual_Commit(void* ptr, u64 size);
void DnMemVirtual_Decommit(void* ptr, u64 size);
void DnMemVirtual_Release(void* ptr);

/*
 * Memory allocator
 */

typedef struct DnMemAllocator DnMemAllocator;

typedef void* (DnMemAllocatorAllocFunc)(DnMemAllocator* allocator, u64 size);
typedef void* (DnMemAllocatorReallocFunc)(DnMemAllocator* allocator, void* pointer, u64 size);
typedef void (DnMemAllocatorFreeFunc)(DnMemAllocator* allocator, void* pointer);

typedef struct DnMemAllocator {
  DnMemAllocatorAllocFunc* alloc;
  DnMemAllocatorReallocFunc* realloc;
  DnMemAllocatorFreeFunc* free;
  void* context;
} DnMemAllocator;

bool DnMemAllocators_Init();
void DnMemAllocators_Deinit();

const DnMemAllocator* DnMemAllocatorMalloc_Get();

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
