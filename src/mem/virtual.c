#include "dn/mem/virtual.h"

u64 DnMemVirtualPageSize() {
  static u64 pageSize = 0;

  if (pageSize == 0) {
    SYSTEM_INFO systemInfo = {};
    GetSystemInfo(&systemInfo);
    pageSize = systemInfo.dwPageSize;
  }

  return pageSize;
}

void* DnMemVirtualReserve(u64 size) {
  return VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);
}

bool DnMemVirtualCommit(void* ptr, u64 size) {
  return VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE) != nullptr;
}

void DnMemVirtualDecommit(void* ptr, u64 size) {
  DN_ASSERT_EVALUATE(VirtualFree(ptr, size, MEM_DECOMMIT) != 0);
}

void DnMemVirtualRelease(void* ptr) {
  DN_ASSERT_EVALUATE(VirtualFree(ptr, 0, MEM_RELEASE) != 0);
}
