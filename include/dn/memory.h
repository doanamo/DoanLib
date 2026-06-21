#pragma once

#include "shared.h"
#include "math.h"

// ========================================================================== //
//   Memory Defines                                                           //
// ========================================================================== //

/*
 * Defines whether debug memory patterns are enabled to aid debugging. Such
 * patterns are used to mark memory regions that have been e.g. allocated or
 * freed with easily recognizable patterns. Very useful when debugging both live
 * processes and also memory dumps.
 */
#define DN_MEM_PATTERNS_ENABLED !DN_CONFIG_RELEASE

// ========================================================================== //
//   Memory Constants                                                         //
// ========================================================================== //

/*
 * Default memory alignment in bytes for memory allocations, imposed by C
 * standard library, which corresponds to the alignment of the largest primitive
 * scalar type available on the platform.
 */
constexpr u64 DnMem_DefaultAlignment = alignof(max_align_t);

/*
 * Expected system memory page size in bytes that is used to align virtual
 * memory address reservations and commits to the physical memory. Determines
 * the effective granularity of memory allocations and deallocations.
 */
constexpr u64 DnMem_SystemPageSize = 4096;

/*
 * Debugging memory fill patterns.
 */
#if DN_MEM_PATTERNS_ENABLED
  // Pattern used to fill new memory after allocation.
  constexpr u8 DnMem_PatternAllocated = 0xCD;

  // Pattern used to fill existing memory after deallocation.
  constexpr u8 DnMem_PatternFreed = 0xDD;

  // Pattern used to fill padding in memory that is not meant to be used.
  constexpr u8 DnMem_PatternPadding = 0xFD;
#endif

// ========================================================================== //
//   Memory Initialization                                                    //
// ========================================================================== //

/*
 * Configuration struct for memory initialization.
 */
typedef struct DnMemConfig {
  u64 tempReservedSize;
} DnMemConfig;

/*
 * Initializes the memory system with the given configuration. Should be called
 * only once at the very beginning of the program, before any memory operations.
 */
bool DnMem_Init(const DnMemConfig* config);

/*
 * Deinitializes the memory system. Should be called only once at the very end
 * of the program, after all memory operations. With debugging enabled, may
 * perform additional checks to validate memory integrity (e.g. checking for
 * memory leaks).
 */
void DnMem_Deinit();

// ========================================================================== //
//   Memory Common Macros                                                     //
// ========================================================================== //

/*
 * Converts a value in kilobytes (consisting of 1024 bytes each) to bytes.
 * Returns an integer value.
 */
#define DN_MEM_KB(kilobytes) ((u64)(kilobytes) << 10)

/*
 * Converts a value in megabytes (consisting of 1024 kilobytes each) to bytes.
 * Returns an integer value.
 */
#define DN_MEM_MB(megabytes) ((u64)(megabytes) << 20)

/*
 * Converts a value in gigabytes (consisting of 1024 megabytes each) to bytes.
 * Returns an integer value.
 */
#define DN_MEM_GB(gigabytes) ((u64)(gigabytes) << 30)

/*
 * Converts a value of bytes to kilobytes (consisting of 1024 bytes each).
 * Returns a floating-point value which may represent fraction of a kilobyte.
 */
#define DN_MEM_TO_KB(bytes) ((f64)(bytes) / (1ull << 10))

/*
 * Converts a value of bytes to megabytes (consisting of 1024 kilobytes each).
 * Returns a floating-point value which may represent fraction of a megabyte.
 */
#define DN_MEM_TO_MB(bytes) ((f64)(bytes) / (1ull << 20))

/*
 * Converts a value of bytes to gigabytes (consisting of 1024 megabytes each).
 * Returns a floating-point value which may represent fraction of a gigabyte.
 */
#define DN_MEM_TO_GB(bytes) ((f64)(bytes) / (1ull << 30))

/*
 * Check whether given value is aligned to multiple of the specified size.
 * Assumes that the alignment is a power of 2. Can also be used with pointers.
 */
#define DN_MEM_IS_ALIGNED(size, alignment) \
  ({ \
    u64 _size = (size); \
    u64 _alignment = (alignment); \
    DN_ASSERT(DN_IS_POW2(_alignment)); \
    (_size & (_alignment - 1)) == 0; \
  })

/*
 * Aligns the given size up to the nearest multiple of the specified alignment.
 * Assumes that the alignment is a power of 2. Can also be used with pointers.
 */
#define DN_MEM_ALIGN_UP(size, alignment) \
  ({ \
    u64 _size = (size); \
    u64 _alignment = (alignment); \
    DN_ASSERT(DN_IS_POW2(_alignment)); \
    ((_size) + (_alignment - 1)) & ~(_alignment - 1); \
  })

// ========================================================================== //
//   Memory Allocation Macros                                                 //
// ========================================================================== //

/*
 * Shorthand macro for allocating memory from an allocator with a given size and
 * alignment.
 */
#define DN_MEM_ALLOC(allocator, size, alignment) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->alloc(_allocator, size, alignment); \
  })

  /*
   * Shorthand macro for reallocating memory from an allocator with a given size
   * and alignment. Accepts a null allocation pointer, in which case a new
   * memory block is allocated. Similarly, passing a zero size will result in
   * deallocation routine being called instead. Returns a pointer to a memory
   * block that may have been shrunk or expanded in-place, or to a new memory
   * block if the existing one could not be resized. In the latter case, the
   * original block's contents are copied to the new block before the original
   * is freed. You should always assume that pointers to the reallocated memory
   * are invalidated.
   */
  #define DN_MEM_REALLOC(allocator, allocation, size, alignment) \
    ({ \
      const DnMemAllocator* _allocator = allocator; \
      _allocator->realloc(_allocator, allocation, size, alignment); \
    })

/*
 * Shorthand macro for freeing memory from an allocator.
 */
#define DN_MEM_FREE(allocator, allocation) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->free(_allocator, allocation); \
  })

/*
 * Shorthand macro for allocating memory from an allocator for a single instance
 * of a given type.
 */
#define DN_MEM_ALLOC_TYPE(allocator, type) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    (type*)_allocator->alloc(_allocator, sizeof(type), alignof(type)); \
  })

/*
 * Shorthand macro for allocating memory from an allocator for an array of a
 * given type and element count.
 */
#define DN_MEM_ALLOC_TYPES(allocator, type, count) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    (type*)_allocator->alloc(_allocator, sizeof(type) * (count), alignof(type)); \
  })

/*
 * Shorthand macro for reallocating memory from an allocator for an array of a
 * given type and element count. See DN_MEM_REALLOC() for additional remarks on
 * reallocation.
 */
#define DN_MEM_REALLOC_TYPES(allocator, allocation, type, count) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    (type*)_allocator->realloc(_allocator, allocation, sizeof(type) * (count), alignof(type)); \
  })

// ========================================================================== //
//   Memory Allocator                                                         //
// ========================================================================== //

/*
 * Memory allocator method declarations. These correspond to
 * DN_MEM_ALLOC/REALLOC/FREE() family of macros which invoke these functions
 * from allocator interface.
 */
typedef struct DnMemAllocator DnMemAllocator;
typedef void* (DnMemAllocatorAllocFunc)(const DnMemAllocator* allocator, u64 size, u64 alignment);
typedef void* (DnMemAllocatorReallocFunc)(const DnMemAllocator* allocator, void* pointer, u64 size, u64 alignment);
typedef void (DnMemAllocatorFreeFunc)(const DnMemAllocator* allocator, void* pointer);

/*
 * Memory allocator interface which can be invoked through family of
 * DN_MEM_ALLOC/REALLOC/FREE() macros for ease of use.
 */
typedef struct DnMemAllocator {
  DnMemAllocatorAllocFunc* alloc;
  DnMemAllocatorReallocFunc* realloc;
  DnMemAllocatorFreeFunc* free;
  void* context;
} DnMemAllocator;

/*
 * Default general purpose memory allocator.
 */
extern const DnMemAllocator* const g_dnMemAllocatorDefault;

/*
 * Standard C library malloc memory allocator.
 */
extern const DnMemAllocator* const g_dnMemAllocatorMalloc;

// ========================================================================== //
//   Memory Virtual                                                           //
// ========================================================================== //

/*
 * Reserves a region of virtual address space of the given size without
 * committing any physical memory to it. The size is rounded up to a multiple of
 * the system page size. Returns the base address of the reserved region, or
 * null if the reservation failed. Reserved memory cannot be accessed until it
 * is committed via DnMemVirtual_Commit().
 */
void* DnMemVirtual_Reserve(u64 size);

/*
 * Commits physical memory to a previously reserved region, making the specified
 * range accessible for use. The page address and size are expected to align
 * with the system page size. Null page address can be specified to
 * automatically reserve address space in single system call, which may be
 * faster than calling DnMemVirtual_Reserve() separately.
 */
void* DnMemVirtual_Commit(void* page, u64 size);

/*
 * Decommits physical memory from a previously committed region, releasing the
 * backing physical memory while keeping the address space reserved. The page
 * address and size are expected to align with the system page size. The memory
 * may be recommitted later via DnMemVirtual_Commit().
 */
void DnMemVirtual_Decommit(void* page, u64 size);

/*
 * Releases an entire region of virtual address space that was previously
 * reserved, freeing both the reservation and any committed physical memory. The
 * page address must be the base address returned by the original reservation.
 */
void DnMemVirtual_Release(void* page);

// ========================================================================== //
//   Memory Arena                                                             //
// ========================================================================== //

typedef struct DnMemArena {
  void* address;
  u64 reservedSize;
  u64 committedSize;
  u64 usedSize;
} DnMemArena;

bool DnMemArena_Init(DnMemArena* arena, u64 reserveSize);
void* DnMemArena_Push(DnMemArena* arena, u64 allocationSize);
void DnMemArena_Pop(DnMemArena* arena, u64 allocationSize);
void DnMemArena_Reset(DnMemArena* arena, bool decommit);
void DnMemArena_Deinit(DnMemArena* arena);

// ========================================================================== //
//   Memory Temporary                                                         //
// ========================================================================== //

typedef struct DnMemTempScope {
  u64 savedUsedSize;
  bool valid;
} DnMemTempScope;

DnMemTempScope DnMemTemp_PushScope();
void DnMemTemp_PopScope(DnMemTempScope* scope);

extern const DnMemAllocator* const g_dnMemAllocatorTemp;
