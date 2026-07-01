#include "dn/memory.h"
#include "dn/shared.h"

// == MEMORY ARENA CONSTANTS ================================================ //

constexpr u64 DnMemArena_LargeSizeThreshold = DnMemLarge_SizeThreshold;

// == MEMORY ARENA STRUCTS ================================================== //

typedef struct DnMemArenaAllocation {
  u64 size;
} DnMemArenaAllocation;

typedef struct DnMemArenaChunk {
  struct DnMemArenaChunk* next;
  u8* free;
  u8* end;
} DnMemArenaChunk;

typedef struct DnMemArena {
  DnMemAllocator allocator;
  u64 chunkSize;

  // Linked list of chunks starting from oldest and ending with the newest.
  // Points at current free chunk. Will never be null, because oldest chunk is
  // always the initial arena chunk. Can be reset to the initial chunk in the
  // list via DnMemArena_GetInitialChunk().
  DnMemArenaChunk* chunks;

  // Linked list of oversized chunks (chonks) that points at the newest chonk.
  // Chonks are not reused and are sized to contain only single large allocation
  // that does not fit the default chunk size or exceeds large size boundary to
  // not waste space in regular chunks that are suited for many smaller
  // allocations. Will be null if no oversized allocations have been made.
  DnMemArenaChunk* chonks;
} DnMemArena;

// == MEMORY ARENA CHUNK ==================================================== //

static u64 DnMemArena_CalcMinChunkSize(u64 size, u64 alignment) {
  return sizeof(DnMemArenaChunk) + sizeof(DnMemArenaAllocation) + alignment + size;
}

static DnMemArenaChunk* DnMemArena_GetInitialChunk(DnMemArena* arena) {
  return (DnMemArenaChunk*)((u8*)arena + sizeof(DnMemArena));
}

static u8* DnMemArena_GetChunkBeginPointer(DnMemArenaChunk* chunk) {
  return (u8*)chunk + sizeof(DnMemArenaChunk);
}

static void DnMemArena_InitChunk(DnMemArenaChunk* chunk, u64 chunkSize) {
  *chunk = (DnMemArenaChunk) {
    .next = nullptr,
    .free = DnMemArena_GetChunkBeginPointer(chunk),
    .end = (u8*)chunk + chunkSize,
  };
}

static DnMemArenaChunk* DnMemArena_CreateChunk(u64 chunkSize) {
  u8* chunkMemory = (u8*)DnMemVirtual_Commit(nullptr, chunkSize);
  DN_ASSERT_ALWAYS(chunkMemory);

  DnMemArenaChunk* chunk = (DnMemArenaChunk*)chunkMemory;
  DnMemArena_InitChunk(chunk, chunkSize);
  return chunk;
}

// == MEMORY ARENA ALLOCATOR ================================================ //

static DnMemArenaAllocation* DnMemArena_GetAllocationHeader(void* allocation) {
  return (DnMemArenaAllocation*)((u8*)allocation - sizeof(DnMemArenaAllocation));
}

static void* DnMemArena_Alloc(const DnMemAllocator* allocator, u64 size, u64 alignment) {
  DN_ASSERT(size > 0);
  size = DN_MEM_ALIGN_UP(size, DnMem_DefaultAlignment);
  alignment = DN_MAX(alignment, DnMem_DefaultAlignment);
  DN_ASSERT(DN_MEM_IS_ALIGNED(alignment, DnMem_DefaultAlignment));

  DN_ASSERT(allocator);
  DnMemArena* arena = (DnMemArena*)allocator->context;
  DN_ASSERT(arena);

  // Initially points at the current chunk from free list, but may end up
  // pointing at a different chunk or an oversized chonk outside the free list.
  DnMemArenaChunk* chunk = arena->chunks;
  DN_ASSERT(chunk);

#if DN_ASSERT_ENABLED
  bool chunkCreated = false;
#endif

  u8* address;
  while (true) {
    // Align pointer and see if it fits within the current chunk.
    // Account for the allocation header placed before aligned address.
    DN_ASSERT(DN_MEM_IS_ALIGNED((u64)chunk->free, DnMem_DefaultAlignment));
    address = (u8*)DN_MEM_ALIGN_UP((u64)chunk->free + sizeof(DnMemArenaAllocation), alignment);
    if (address + size <= chunk->end)
      break;

    // Ensure new chunk guaranteed to be large enough for the allocation.
    DN_ASSERT(!chunkCreated);

    // Try next chunk if there is one available.
    // #todo: There may still be some space left in the current chunk that
    // could be used. Instead of replacing it with next chunk, keep trying to
    // allocate from it again in the future. Keep in mind that scopes that will
    // revert state of chunks need to be aware of this behavior. This can have a
    // really bad worst case scenarion when allocating very large size that will
    // throw away whole free list.
    if (chunk->next) {
      chunk = chunk->next;
      arena->chunks = chunk;
      continue;
    }

    // Calculate minimum chunk size that we need for this allocation in case
    // requested size overflows default chunk size. If it does, allocate chunk
    // dedicated only for this allocation and put it in dedicated non-free list.
    u64 minimumChunkSize = DnMemArena_CalcMinChunkSize(size, alignment);
    bool oversizedChonk = minimumChunkSize > arena->chunkSize || size > DnMemArena_LargeSizeThreshold;

    // Allocate new chunk address space.
    u64 chunkSize = oversizedChonk ? minimumChunkSize : arena->chunkSize;
    chunk = DnMemArena_CreateChunk(chunkSize);

  #if DN_ASSERT_ENABLED
    chunkCreated = true;
  #endif

    if (!oversizedChonk) {
      // Append new chunk to the end of the free list.
      DN_ASSERT(!arena->chunks->next);
      arena->chunks->next = chunk;
      arena->chunks = chunk;
    }
    else {
      // Prepend oversized chonk at the front of the non-free list.
      chunk->next = arena->chonks;
      arena->chonks = chunk;
    }
  }

  // Clear memory with debug pattern.
#if DN_MEM_PATTERNS_ENABLED
  memset(chunk->free, DnMem_PatternPadding, (u64)(address - chunk->free));
  memset(address, DnMem_PatternAllocated, size);
#endif

  // Store header with size information before aligned address.
  DnMemArenaAllocation* header = DnMemArena_GetAllocationHeader(address);
  *header = (DnMemArenaAllocation) {
    .size = size,
  };

  // Move free pointer within the chunk past allocated space.
  chunk->free = address + size;

  return address;
}

static void DnMemArena_Free(const DnMemAllocator* allocator, void* allocation) {
  DN_UNUSED(allocator);
  DN_UNUSED(allocation);

  if (allocation == nullptr)
    return;

  // Deallocating from an arena is a no-op. Freeing allocations is accomplished
  // via scopes that rollback an arena to previous state.

#if DN_MEM_PATTERNS_ENABLED
  DnMemArenaAllocation* header = DnMemArena_GetAllocationHeader(allocation);
  memset(allocation, DnMem_PatternFreed, header->size);
#endif
}

static void* DnMemArena_Realloc(const DnMemAllocator* allocator, void* allocation, u64 size, u64 alignment) {
  DN_ASSERT(allocator);

  if (size == 0) {
    DnMemArena_Free(allocator, allocation);
    return nullptr;
  }

  // #todo: Optimize for resizing last allocation in chunk without creating a
  // new allocation block on top.
  void* reallocation = DnMemArena_Alloc(allocator, size, alignment);
  DN_ASSERT_ALWAYS(reallocation);

  if (allocation) {
    DnMemArenaAllocation* header = DnMemArena_GetAllocationHeader(allocation);
    memcpy(reallocation, allocation, DN_MIN(header->size, size));
    DnMemArena_Free(allocator, allocation);
  }

  return reallocation;
}

const DnMemAllocator* DnMemArena_GetAllocator(const DnMemArena* arena) {
  DN_ASSERT(arena);
  return &arena->allocator;
}

// == MEMORY ARENA INITIALIZATION =========================================== //

DnMemArena* DnMemArena_Create(u64 chunkSize) {
  DN_ASSERT(chunkSize > 0);

  chunkSize = DN_MEM_ALIGN_UP(chunkSize, DnMem_ReservationGranularity);
  u8* memory = (u8*)DnMemVirtual_Commit(nullptr, chunkSize);
  DN_ASSERT_ALWAYS(memory);

  DnMemArena* arena = (DnMemArena*)memory;
  *arena = (DnMemArena) {
    .allocator = (DnMemAllocator) {
      .alloc = DnMemArena_Alloc,
      .realloc = DnMemArena_Realloc,
      .free = DnMemArena_Free,
      .context = arena,
    },
    .chunkSize = chunkSize,
    .chunks = DnMemArena_GetInitialChunk(arena),
    .chonks = nullptr,
  };

  // Initial chunk will be slightly smaller due to the arena header overhead.
  u64 initialChunkSize = chunkSize - sizeof(DnMemArena);
  DnMemArena_InitChunk(arena->chunks, initialChunkSize);

  return arena;
}

void DnMemArena_Destroy(DnMemArena* arena) {
  DN_ASSERT(arena);

  // Initial chunk is tied to the arena itself, so we skip it when freeing.
  DnMemArenaChunk* chunk = DnMemArena_GetInitialChunk(arena);
  DN_ASSERT(chunk);

  chunk = chunk->next;
  while (chunk) {
    DnMemArenaChunk* next = chunk->next;
    DnMemVirtual_Release(chunk);
    chunk = next;
  }

  DnMemArenaChunk* chonk = arena->chonks;
  while (chonk) {
    DnMemArenaChunk* next = chonk->next;
    DnMemVirtual_Release(chonk);
    chonk = next;
  }

  DnMemVirtual_Release(arena);
}

// == MEMORY ARENA SCOPE ==================================================== //

typedef struct DnMemArenaScopeOpaque {
  DnMemArena* arena;
  DnMemArenaChunk* chunk;
  DnMemArenaChunk* chonk;
  u8* chunkFree;
} DnMemArenaScopeOpaque;

static_assert(sizeof(DnMemArenaScopeOpaque) == sizeof(DnMemArenaScope));
static_assert(alignof(DnMemArenaScopeOpaque) == alignof(DnMemArenaScope));

DnMemArenaScope DnMemArena_PushScope(DnMemArena* arena) {
  DN_ASSERT(arena);

  DnMemArenaScopeOpaque scope = {
    .arena = arena,
    .chunk = arena->chunks,
    .chonk = arena->chonks,
    .chunkFree = arena->chunks->free,
  };

  return *(DnMemArenaScope*)&scope;
}

void DnMemArena_PopScope(DnMemArenaScope* scope) {
  DnMemArenaScopeOpaque* opaque = (DnMemArenaScopeOpaque*)scope;
  DnMemArena* arena = opaque->arena;
  DN_ASSERT(arena);

  // Empty subsequent chunks that were allocated past chunk saved in scope.
  // We only need to reset subsequent chunks up to the current one.
  DnMemArenaChunk* chunk = opaque->chunk;
  while (chunk != arena->chunks) {
    DN_ASSERT(chunk->next);
    chunk = chunk->next;
    chunk->free = DnMemArena_GetChunkBeginPointer(chunk);
  }

  // Make previous chunk current and restore its free pointer.
  arena->chunks = opaque->chunk;
  arena->chunks->free = opaque->chunkFree;

  // Free chonks that were not previously present in this scope.
  DnMemArenaChunk* chonk = arena->chonks;
  while (chonk != opaque->chonk) {
    DN_ASSERT(chonk);
    DnMemArenaChunk* next = chonk->next;
    DnMemVirtual_Release(chonk);
    chonk = next;
  }

  arena->chonks = opaque->chonk;
}
