#include "dn/memory.h"

bool DnMemVirtual_Init();
void DnMemVirtual_Deinit();

bool DnMemTemp_Init();
void DnMemTemp_Deinit();

bool DnMem_Init() {
  DN_LOG_INFO("System memory page size: %llu", DnMem_SystemPageSize);
  DN_LOG_INFO("Default memory alignment: %llu", DnMem_DefaultAlignment);

  if (!DnMemVirtual_Init()) {
    return false;
  }

  if (!DnMemTemp_Init()) {
    return false;
  }

  return true;
}

void DnMem_Deinit() {
  DnMemTemp_Deinit();
  DnMemVirtual_Deinit();
}
