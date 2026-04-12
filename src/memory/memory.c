#include "dn/memory.h"

bool DnMemVirtual_ValidatePageSize();

bool DnMem_Init() {
  DN_LOG_INFO("System memory page size: %llu", DnMem_SystemPageSize);
  DN_LOG_INFO("Default memory alignment: %llu", DnMem_DefaultAlignment);

  if (!DnMemVirtual_ValidatePageSize()) {
    return false;
  }

  return true;
}

void DnMem_Deinit() {
}

extern DnMemAllocator g_dnMemAllocatorMalloc;
const DnMemAllocator* DnMemAllocatorDefault_Get() {
  return &g_dnMemAllocatorMalloc;
}
