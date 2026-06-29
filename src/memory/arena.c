#include "dn/memory.h"

// == ARENA STRUCTS ========================================================= //

typedef struct DnMemArenaChunk {
  struct DnMemArenaChunk* next;
  u8* free;
  u8* end;
} DnMemArenaChunk;

typedef struct DnMemArena {
  DnMemAllocator allocator;
  DnMemArenaChunk* chunks;
  u64 chunkSize;
} DnMemArena;

// == ARENA CHUNK =========================================================== //

static DnMemArenaChunk* DnMemArena_GetInitialChunk(DnMemArena* arena) {
  return (DnMemArenaChunk*)((u8*)arena + sizeof(DnMemArena));
}

static u8* DnMemArena_GetUsableChunkSpace(DnMemArenaChunk* chunk) {
  return (u8*)chunk + sizeof(DnMemArenaChunk);
}

static void DnMemArena_InitChunk(DnMemArenaChunk* chunk, u64 chunkSize) {
  *chunk = (DnMemArenaChunk) {
    .next = nullptr,
    .free = DnMemArena_GetUsableChunkSpace(chunk),
    .end = (u8*)chunk + chunkSize,
  };
}

// == ARENA CREATION ======================================================== //

static void* DnMemArena_Alloc(const DnMemAllocator* allocator, u64 size, u64 alignment);
static void* DnMemArena_Realloc(const DnMemAllocator* allocator, void* ptr, u64 size, u64 alignment);
static void DnMemArena_Free(const DnMemAllocator* allocator, void* ptr);

DnMemArena* DnMemArena_Create(u64 chunkSize) {
  DN_ASSERT(chunkSize > 0);
  u64 initialArenaSize = chunkSize;
  initialArenaSize += sizeof(DnMemArena);
  initialArenaSize += sizeof(DnMemArenaChunk);
  initialArenaSize = DN_MEM_ALIGN_UP(initialArenaSize, DnMem_SystemPageSize);
  chunkSize = initialArenaSize - sizeof(DnMemArena);

  u8* memory = (u8*)DnMemVirtual_Commit(nullptr, initialArenaSize);
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
  };

  DnMemArena_InitChunk(arena->chunks, chunkSize);

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

  DnMemVirtual_Release(arena);
}

// == ARENA ALLOCATOR ======================================================= //

typedef struct DnMemArenaAllocation {
  u64 size;
} DnMemArenaAllocation;

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

  DnMemArenaChunk* chunk = arena->chunks;
  DN_ASSERT(chunk);

  u8* address;
  while (true) {
    // Align pointer and see if it fits within the current chunk.
    // Account for the allocation header size before aligning.
    DN_ASSERT(DN_MEM_IS_ALIGNED((u64)chunk->free, DnMem_DefaultAlignment));
    address = (u8*)DN_MEM_ALIGN_UP((u64)chunk->free + sizeof(DnMemArenaAllocation), alignment);
    if (address + size <= chunk->end)
      break;

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
    // requested size overflows default chunk size.
    u64 minimumChunkSize = sizeof(DnMemArenaChunk) + sizeof(DnMemArenaAllocation) + alignment + size;
    minimumChunkSize = DN_MEM_ALIGN_UP(minimumChunkSize, DnMem_SystemPageSize);

    // Allocate new chunk if the current chunk has insufficient space.
    // #todo: Large allocations could over-reserve address space to make
    // reallocations faster. Could start with the lowest reservation alignment
    // of 64KB and increase by power of two as needed. Could make a dedicated
    // large size allocator to manage large allocations separately.
    u64 chunkSize = DN_MAX(arena->chunkSize, minimumChunkSize);
    u8* chunkMemory = (u8*)DnMemVirtual_Commit(nullptr, chunkSize);
    DN_ASSERT_ALWAYS(chunkMemory);

    // Initialize the chunk and add it to the arena as current chunk.
    chunk = (DnMemArenaChunk*)chunkMemory;
    DnMemArena_InitChunk(chunk, chunkSize);
    chunk->next = arena->chunks;
    arena->chunks = chunk;
  }

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

// == ARENA SCOPE =========================================================== //

typedef struct DnMemArenaScopeOpaque {
  DnMemArena* arena;
  DnMemArenaChunk* chunk;
  u8* free;
} DnMemArenaScopeOpaque;

static_assert(sizeof(DnMemArenaScopeOpaque) == sizeof(DnMemArenaScope));
static_assert(alignof(DnMemArenaScopeOpaque) == alignof(DnMemArenaScope));

DnMemArenaScope DnMemArena_PushScope(DnMemArena* arena) {
  DN_ASSERT(arena);

  DnMemArenaScopeOpaque scope = {
    .arena = arena,
    .chunk = arena->chunks,
    .free = arena->chunks->free,
  };

  return *(DnMemArenaScope*)&scope;
}

void DnMemArena_PopScope(DnMemArenaScope* scope) {
  DnMemArenaScopeOpaque* opaque = (DnMemArenaScopeOpaque*)scope;
  DnMemArena* arena = opaque->arena;
  DN_ASSERT(arena);

  // Rollback current chunk to the state when scope was created.
  arena->chunks = opaque->chunk;
  arena->chunks->free = opaque->free;

  // Empty subsequent chunks that were allocated from after scope was created.
  DnMemArenaChunk* chunk = arena->chunks->next;
  while (chunk) {
    chunk->free = DnMemArena_GetUsableChunkSpace(chunk);
    chunk = chunk->next;
  }
}
