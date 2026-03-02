#include "dn/memory.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

u64 DnMemVirtual_GetPageSize() {
  static u64 pageSize = 0;

  if (pageSize == 0) {
    SYSTEM_INFO systemInfo = {};
    GetSystemInfo(&systemInfo);
    pageSize = systemInfo.dwPageSize;
  }

  return pageSize;
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
