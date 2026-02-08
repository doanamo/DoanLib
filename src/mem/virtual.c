#include "dn/shared.h"
#include "dn/mem/virtual.h"

u32 DnMemVirtualPageSize() {
  SYSTEM_INFO systemInfo = {};
  GetSystemInfo(&systemInfo);
  return systemInfo.dwPageSize;
}

void* DnMemVirtualReserve(u64 size) {
  return VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_READWRITE);
}

bool DnMemVirtualCommit(void* ptr, u64 size) {
  return VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE) != nullptr;
}

bool DnMemVirtualDecommit(void* ptr, u64 size) {
  return VirtualFree(ptr, size, MEM_RELEASE) != 0;
}

bool DnMemVirtualRelease(void* ptr) {
  return VirtualFree(ptr, 0, MEM_RELEASE) != 0;
}
