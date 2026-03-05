#include "dn/memory.h"

bool DnMemAllocators_Init();
void DnMemAllocators_Deinit();

bool DnMem_Init() {
  DN_LOG_INFO("System memory page size: %llu", DnMemVirtual_GetPageSize());
  DN_LOG_INFO("Default memory alignment: %llu", alignof(max_align_t));

  if (!DnMemAllocators_Init()) {
    return false;
  }

  return true;
}

void DnMem_Deinit() {
  DnMemAllocators_Deinit();
}
