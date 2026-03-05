#include "dn/memory.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

bool DnMemVirtual_Init() {
  SYSTEM_INFO systemInfo = {};
  GetSystemInfo(&systemInfo);

  if (DnMem_SystemPageSize != systemInfo.dwPageSize) {
    DN_LOG_ERROR("Unexpected system memory page size: expected %llu, got %llu", DnMem_SystemPageSize, systemInfo.dwPageSize);
    return false;
  }

  return true;
}

void DnMemVirtual_Deinit() {
}

void* DnMemVirtual_Reserve(u64 size) {
  return VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);
}

bool DnMemVirtual_Commit(void* page, u64 size) {
  return VirtualAlloc(page, size, MEM_COMMIT, PAGE_READWRITE) != nullptr;
}

void DnMemVirtual_Decommit(void* page, u64 size) {
  DN_ASSERT_EVALUATE(VirtualFree(page, size, MEM_DECOMMIT) != 0);
}

void DnMemVirtual_Release(void* page) {
  DN_ASSERT_EVALUATE(VirtualFree(page, 0, MEM_RELEASE) != 0);
}
