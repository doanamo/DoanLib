#include "dn/mem/virtual.h"

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

bool DnMemVirtual_Commit(void* ptr, u64 size) {
  return VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE) != nullptr;
}

void DnMemVirtual_Decommit(void* ptr, u64 size) {
  DN_ASSERT_EVALUATE(VirtualFree(ptr, size, MEM_DECOMMIT) != 0);
}

void DnMemVirtual_Release(void* ptr) {
  DN_ASSERT_EVALUATE(VirtualFree(ptr, 0, MEM_RELEASE) != 0);
}
