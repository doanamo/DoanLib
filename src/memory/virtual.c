#include "dn/memory.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// ========================================================================== //

bool DnMemVirtual_Init() {
  SYSTEM_INFO systemInfo = {};
  GetSystemInfo(&systemInfo);

  if (DnMem_SystemPageSize != systemInfo.dwPageSize) {
    DN_LOG_ERROR("Unexpected system memory page size: expected %llu, got %lu",
      DnMem_SystemPageSize, systemInfo.dwPageSize);
    return false;
  }

  return true;
}

// ========================================================================== //

void DnMemVirtual_Deinit() {
  // #todo: Implement debug checks for virtual memory page integrity (e.g.
  // whether all virtual pages have been decommitted and released).
}

// ========================================================================== //

void* DnMemVirtual_Reserve(u64 size) {
  // On Windows, virtual memory adress space always operates on pages, so not
  // having size aligned to page boundaries will not have any adverse effects
  // and the call will automatically round up to the nearest page boundary.
  // This check is to ensure that the caller is aware of the above.
  DN_ASSERT(DN_MEM_IS_ALIGNED(size, DnMem_SystemPageSize));
  return VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);
}

// ========================================================================== //

bool DnMemVirtual_Commit(void* page, u64 size) {
  DN_ASSERT(DN_MEM_IS_ALIGNED(size, DnMem_SystemPageSize));
  return VirtualAlloc(page, size, MEM_COMMIT, PAGE_READWRITE) != nullptr;
}

// ========================================================================== //

void DnMemVirtual_Decommit(void* page, u64 size) {
  DN_ASSERT(DN_MEM_IS_ALIGNED(size, DnMem_SystemPageSize));
  DN_ASSERT_EVALUATE(VirtualFree(page, size, MEM_DECOMMIT) != 0);
}

// ========================================================================== //

void DnMemVirtual_Release(void* page) {
  DN_ASSERT_EVALUATE(VirtualFree(page, 0, MEM_RELEASE) != 0);
}
