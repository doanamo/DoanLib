#pragma once

#include "shared.h"
#include "math.h"

// == MEMORY DEFINES ======================================================== //

// Defines whether debug memory patterns are enabled to aid debugging. Such
// patterns are used to mark memory regions that have been e.g. allocated or
// freed with easily recognizable patterns. Very useful when debugging both live
// processes and also memory dumps.
#define DN_MEM_PATTERNS_ENABLED !DN_CONFIG_RELEASE

// == MEMORY COMMON ========================================================= //

// Converts a value in kilobytes (consisting of 1024 bytes each) to bytes.
// Returns an integer value.
#define DN_MEM_KB(kilobytes) ((u64)(kilobytes) << 10)

// Converts a value in megabytes (consisting of 1024 kilobytes each) to bytes.
// Returns an integer value.
#define DN_MEM_MB(megabytes) ((u64)(megabytes) << 20)

// Converts a value in gigabytes (consisting of 1024 megabytes each) to bytes.
// Returns an integer value.
#define DN_MEM_GB(gigabytes) ((u64)(gigabytes) << 30)

// Converts a value of bytes to kilobytes (consisting of 1024 bytes each).
// Returns a floating-point value which may represent fraction of a kilobyte.
#define DN_MEM_TO_KB(bytes) ((f64)(bytes) / (1ull << 10))

// Converts a value of bytes to megabytes (consisting of 1024 kilobytes each).
// Returns a floating-point value which may represent fraction of a megabyte.
#define DN_MEM_TO_MB(bytes) ((f64)(bytes) / (1ull << 20))

// Converts a value of bytes to gigabytes (consisting of 1024 megabytes each).
// Returns a floating-point value which may represent fraction of a gigabyte.
#define DN_MEM_TO_GB(bytes) ((f64)(bytes) / (1ull << 30))

// Check whether given value is aligned to multiple of the specified size.
// Assumes that the alignment is a power of 2. Can also be used with pointers.
#define DN_MEM_IS_ALIGNED(size, alignment) \
  ({ \
    u64 _size = (size); \
    u64 _alignment = (alignment); \
    DN_ASSERT(DN_IS_POW2(_alignment)); \
    (_size & (_alignment - 1)) == 0; \
  })

// Aligns the given size up to the nearest multiple of the specified alignment.
// Assumes that the alignment is a power of 2. Can also be used with pointers.
#define DN_MEM_ALIGN_UP(size, alignment) \
  ({ \
    u64 _size = (size); \
    u64 _alignment = (alignment); \
    DN_ASSERT(DN_IS_POW2(_alignment)); \
    ((_size) + (_alignment - 1)) & ~(_alignment - 1); \
  })

// == MEMORY CONSTANTS ====================================================== //

// Default memory alignment in bytes for memory allocations, imposed by C
// standard library, which corresponds to the alignment of the largest primitive
// scalar type available on the platform.
constexpr u64 DnMem_DefaultAlignment = alignof(max_align_t);

// Granularity of system virtual memory address reservations in bytes. If you
// reserve and manage ranges of memory, there is no reason not to use this
// granurality as the minimum range size.
constexpr u64 DnMem_ReservationGranularity = DN_MEM_KB(64);

// Expected system memory page size in bytes that is used to align virtual
// memory address reservations and commits to the physical memory. Determines
// the effective granularity of memory allocations and deallocations.
constexpr u64 DnMem_SystemPageSize = DN_MEM_KB(4);

// Debugging memory fill patterns.
#if DN_MEM_PATTERNS_ENABLED
  // Pattern used to fill new memory after allocation.
  constexpr u8 DnMem_PatternAllocated = 0xCD;

  // Pattern used to fill existing memory after deallocation.
  constexpr u8 DnMem_PatternFreed = 0xDD;

  // Pattern used to fill padding in memory that is not meant to be used.
  constexpr u8 DnMem_PatternPadding = 0xFD;
#endif

// == MEMORY INITIALIZATION ================================================= //

// Configuration struct for memory initialization.
typedef struct DnMemConfig {
  u64 tempChunkSize;
} DnMemConfig;

// Initializes the memory system with the given configuration. Should be called
// only once at the very beginning of the program, before any memory operations.
bool DnMem_Init(const DnMemConfig* config);

// Deinitializes the memory system. Should be called only once at the very end
// of the program, after all memory operations. With debugging enabled, may
// perform additional checks to validate memory integrity (e.g. checking for
// memory leaks).
void DnMem_Deinit();

// == MEMORY ALLOCATION ====================================================== //

// Shorthand macro for allocating memory from an allocator with a given size and
// alignment.
#define DN_MEM_ALLOC(allocator, size, alignment) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->alloc(_allocator, size, alignment); \
  })

// Shorthand macro for reallocating memory from an allocator with a given size
// and alignment. Accepts a null allocation pointer, in which case a new memory
// block is allocated. Similarly, passing a zero size will result in
// deallocation routine being called instead. Returns a pointer to a memory
// block that may have been shrunk or expanded in-place, or to a new memory
// block if the existing one could not be resized. In the latter case, the
// original block's contents are copied to the new block before the original is
// freed. You should always assume that pointers to the reallocated memory are
// invalidated.
#define DN_MEM_REALLOC(allocator, allocation, size, alignment) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->realloc(_allocator, allocation, size, alignment); \
  })

// Shorthand macro for freeing memory from an allocator.
#define DN_MEM_FREE(allocator, allocation) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->free(_allocator, allocation); \
  })

// Shorthand macro for allocating memory from an allocator for a single instance
// of a given type.
#define DN_MEM_ALLOC_TYPE(allocator, type) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    (type*)_allocator->alloc(_allocator, sizeof(type), alignof(type)); \
  })

// Shorthand macro for allocating memory from an allocator for an array of a
// given type and element count.
#define DN_MEM_ALLOC_TYPES(allocator, type, count) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    (type*)_allocator->alloc(_allocator, sizeof(type) * (count), alignof(type)); \
  })

// Shorthand macro for reallocating memory from an allocator for an array of a
// given type and element count. See DN_MEM_REALLOC() for additional remarks on
// reallocation.
#define DN_MEM_REALLOC_TYPES(allocator, allocation, type, count) \
  ({ \
    const DnMemAllocator* _allocator = allocator; \
    (type*)_allocator->realloc(_allocator, allocation, sizeof(type) * (count), alignof(type)); \
  })

// == MEMORY ALLOCATOR ====================================================== //

typedef struct DnMemAllocator DnMemAllocator;

// Function pointer for memory allocator allocation method.
typedef void* (DnMemAllocatorAllocFunc)(const DnMemAllocator* allocator, u64 size, u64 alignment);

// Function pointer for memory allocator reallocation method.
typedef void* (DnMemAllocatorReallocFunc)(const DnMemAllocator* allocator, void* pointer, u64 size, u64 alignment);

// Function pointer for memory allocator free method.
typedef void (DnMemAllocatorFreeFunc)(const DnMemAllocator* allocator, void* pointer);

// Memory allocator interface which can be invoked through family of
// DN_MEM_ALLOC/REALLOC/FREE() macros for ease of use.
typedef struct DnMemAllocator {
  DnMemAllocatorAllocFunc* alloc;
  DnMemAllocatorReallocFunc* realloc;
  DnMemAllocatorFreeFunc* free;
  void* context;
} DnMemAllocator;

// Default general purpose memory allocator. Used for common allocation cases
// when there are no specialized allocators available for given purpose.
const DnMemAllocator* DnMemAllocator_GetDefault();

// == MEMORY MALLOC ========================================================= //

// Standard C library malloc memory allocator. Should be avoided when possible
// in favor of custom solutions provided by this library. May have uses as
// debugging allocator for analyzing memory usage or finding bugs using third
// party tools.
const DnMemAllocator* DnMemMalloc_GetAllocator();

// == MEMORY VIRTUAL ======================================================== //

// Reserves a region of virtual address space of the given size without
// committing any physical memory to it. The size is rounded up to a multiple of
// the system page size. Returns the base address of the reserved region, or
// null if the reservation failed. Reserved memory cannot be accessed until it
// is committed via DnMemVirtual_Commit().
void* DnMemVirtual_Reserve(u64 size);

// Commits physical memory to a previously reserved region, making the specified
// range accessible for use. The page address and size are expected to align
// with the system page size. Null page address can be specified to
// automatically reserve address space in single system call, which may be
// faster than calling DnMemVirtual_Reserve() separately.
void* DnMemVirtual_Commit(void* page, u64 size);

// Decommits physical memory from a previously committed region, releasing the
// backing physical memory while keeping the address space reserved. The page
// address and size are expected to align with the system page size. The memory
// may be recommitted later via DnMemVirtual_Commit().
void DnMemVirtual_Decommit(void* page, u64 size);

// Releases an entire region of virtual address space that was previously
// reserved, freeing both the reservation and any committed physical memory. The
// page address must be the base address returned by the original reservation.
void DnMemVirtual_Release(void* page);

// Returns the size of the virtual memory region starting at the given page
// address, or 0 if the page address is invalid.
u64 DnMemVirtual_QuerySize(void* page);

// == MEMORY LARGE ========================================================== //

// Threshold size for large allocations.
constexpr u64 DnMemLarge_SizeThreshold = DN_MEM_KB(128);

// Large allocator that puts individual allocations into separate dedicated
// system memory pages for simplicity and lower memory fragmentation.
const DnMemAllocator* DnMemLarge_GetAllocator();

// == MEMORY ARENA ========================================================== //

// Memory arena allocator that represents a set of memory regions of given chunk
// size. The arena behaves like a simple bump allocator, allowing for fast
// allocation and deallocation of all regions at once, at a cost of potentially
// worse memory space utilization when allocations are intended to be freed.
// Freeing allocations is a no-op due to performance reasons. However, scoping
// functionality is provided to allow reverting to a previous state after
// performing a series of temporary allocations. Struct is an opaque type that
// resides on the first page of reserved memory region and inherits allocator
// interface.
typedef struct DnMemArena DnMemArena;

// Allocates a new memory arena with the given chunk size. The arena will
// allocate memory in chunks of the specified size (which will be rounded up to
// next system page size). This size indicates address space usage for each
// chunk and will not reflect actual physical memory usage. The size should be a
// balance between too high value resulting in pressure on address space and too
// low value resulting in frequent chunk allocations.
DnMemArena* DnMemArena_Create(u64 chunkSize);

// Destroys an arena instance, freeing all allocations associated with it.
void DnMemArena_Destroy(DnMemArena* arena);

// Retrieves allocator interface for the arena.
const DnMemAllocator* DnMemArena_GetAllocator(const DnMemArena* arena);

// Represents a scope within an arena, used to record the state of the arena so
// it can be reverted to later, so allocations done within it can be then freed
// all at once when the scope is popped.
typedef struct DnMemArenaScope {
  alignas(DnMem_DefaultAlignment)
  char opaque[32];
} DnMemArenaScope;

// Records the current state of the arena so it can be reverted to later, so
// allocations done within it can be then freed all at once when the scope is
// popped.
DnMemArenaScope DnMemArena_PushScope(DnMemArena* arena);

// Restores the previously recorded state of an arena, unwinding allocations
// that were made since the scope was pushed.
void DnMemArena_PopScope(DnMemArenaScope* scope);

// == MEMORY TEMPORARY ====================================================== //

// Temporary memory allocator that uses an arena for efficient allocation of
// short-lived memory. Very efficient when used in combination with
// DnMemTemp_PushScope()/PopScope() for scoped allocations.
const DnMemAllocator* DnMemTemp_GetAllocator();

// Alias for DnMemArena scope which temporary allocator is backed by.
typedef struct DnMemArenaScope DnMemTempScope;

// Shorthand for calling DnMemArena_PushScope() on the temporary allocator
// backed by an arena.
DnMemTempScope DnMemTemp_PushScope();

// Shorthand for calling DnMemArena_PopScope() on the temporary allocator backed
// by an arena.
void DnMemTemp_PopScope(DnMemTempScope* scope);
