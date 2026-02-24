#pragma once

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
