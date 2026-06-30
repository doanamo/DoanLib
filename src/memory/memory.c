#include "dn/memory.h"

// == MEMORY INITIALIZATION ================================================= //

bool DnMemVirtual_Init();
void DnMemVirtual_Deinit();

bool DnMemTemp_Init(const DnMemConfig* config);
void DnMemTemp_Deinit();

bool DnMem_Init(const DnMemConfig* config) {
  DN_ASSERT(config);

  DN_LOG_INFO("Default memory alignment: %llu bytes", DnMem_DefaultAlignment);

  if (!DnMemVirtual_Init()) {
    return false;
  }

  if (!DnMemTemp_Init(config)) {
    return false;
  }

  return true;
}

void DnMem_Deinit() {
  DnMemTemp_Deinit();
  DnMemVirtual_Deinit();
}

const DnMemAllocator* DnMemAllocator_GetDefault() {
  return DnMemMalloc_GetAllocator();
}
