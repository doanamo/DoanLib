#include "dn/memory.h"

// == TEMPORARY MEMORY INITIALIZATION ======================================= //

static DnMemArena* g_dnMemArenaTemp;
const DnMemAllocator* g_dnMemAllocatorTemp;

bool DnMemTemp_Init(const DnMemConfig* config) {
  u64 tempChunkSize = config->tempChunkSize ? config->tempChunkSize : DN_MEM_MB(64);
  DN_LOG_INFO("Temporary arena chunk size: %.2f MB", DN_MEM_TO_MB(tempChunkSize));

  DN_ASSERT(g_dnMemArenaTemp == nullptr);
  g_dnMemArenaTemp = DnMemArena_Create(tempChunkSize);
  if (!g_dnMemArenaTemp) {
    return false;
  }

  g_dnMemAllocatorTemp = DnMemArena_GetAllocator(g_dnMemArenaTemp);

  return true;
}

void DnMemTemp_Deinit() {
  if (g_dnMemArenaTemp) {
    DnMemArena_Destroy(g_dnMemArenaTemp);
    g_dnMemArenaTemp = nullptr;
  }
}

// == TEMPORARY MEMORY SCOPE ================================================ //

DnMemTempScope DnMemTemp_PushScope() {
  DN_ASSERT(g_dnMemArenaTemp);
  return DnMemArena_PushScope(g_dnMemArenaTemp);
}

void DnMemTemp_PopScope(DnMemTempScope* scope) {
  DN_ASSERT(g_dnMemArenaTemp);
  DnMemArena_PopScope(scope);
}
