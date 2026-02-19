#pragma once

typedef struct DnMemArena {
  void* address;
  u64 reservedSize;
  u64 committedSize;
  u64 usedSize;
} DnMemArena;

bool DnMemArenaInit(DnMemArena* arena, u64 reserveSize);
void* DnMemArenaAlloc(DnMemArena* arena, u64 size);
void DnMemArenaFree(DnMemArena* arena, bool decommit);
void DnMemArenaDeinit(DnMemArena* arena);
