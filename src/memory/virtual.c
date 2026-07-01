#include "dn/memory.h"
#include "dn/system.h"
#include <winnt.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// == VIRTUAL MEMORY INITIALIZATION ========================================= //

bool DnMemVirtual_Init() {
  SYSTEM_INFO systemInfo = {};
  GetSystemInfo(&systemInfo);

  if (DnMem_SystemPageSize != systemInfo.dwPageSize) {
    DN_LOG_ERROR("Unexpected system memory page size: expected %llu, got %lu",
      DnMem_SystemPageSize, systemInfo.dwPageSize);
    return false;
  }

  DN_LOG_INFO("System memory page size: %llu bytes", DnMem_SystemPageSize);

  if (DnMem_ReservationGranularity != systemInfo.dwAllocationGranularity) {
    DN_LOG_ERROR("Unexpected system memory reservation granularity: expected %llu, got %lu",
      DnMem_ReservationGranularity, systemInfo.dwAllocationGranularity);
    return false;
  }

  DN_LOG_INFO("System memory reservation granularity: %llu bytes", DnMem_ReservationGranularity);

  return true;
}

void DnMemVirtual_Deinit() {
  // #todo: Implement debug checks for virtual memory page integrity (e.g.
  // whether all virtual pages have been decommitted and released).
}

// == VIRTUAL MEMORY ALLOCATION ============================================= //

void* DnMemVirtual_Reserve(u64 size) {
  // On Windows, virtual memory adress space always operates on pages, so not
  // having size aligned to page boundaries will not have any adverse effects
  // and the call will automatically round up to the nearest page boundary.
  // This check is to ensure that the caller is aware of the above.
  DN_ASSERT(DN_MEM_IS_ALIGNED(size, DnMem_SystemPageSize));

  void* address = VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);
  if (DN_UNLIKELY(address == nullptr)) {
    DN_LOG_ERROR("Failed to reserve virtual memory");
    DnSysWin32_LogLastError();
  }

  return address;
}

u64 DnMemVirtual_QuerySize(void* page) {
  DN_ASSERT(page != nullptr);

  // Note: this only works for memory regions sharing same attributes, that were
  // created using same initial call to VirtualAlloc. If pages were e.g.
  // commited incrementally, the returned size may not be accurate.
  MEMORY_BASIC_INFORMATION info;
  if (DN_UNLIKELY(VirtualQuery(page, &info, sizeof(info)) == 0)) {
    DN_LOG_ERROR("Failed to query virtual memory");
    DnSysWin32_LogLastError();
    return 0;
  }

  return info.RegionSize;
}

void DnMemVirtual_Decommit(void* page, u64 size) {
  DN_ASSERT(page != nullptr);
  DN_ASSERT(DN_MEM_IS_ALIGNED(size, DnMem_SystemPageSize));

  if (DN_UNLIKELY(VirtualFree(page, size, MEM_DECOMMIT) == 0)) {
    DN_LOG_ERROR("Failed to decommit virtual memory");
    DnSysWin32_LogLastError();
  }
}

void DnMemVirtual_Release(void* page) {
  DN_ASSERT(page != nullptr);

  if (DN_UNLIKELY(VirtualFree(page, 0, MEM_RELEASE) == 0)) {
    DN_LOG_ERROR("Failed to release virtual memory");
    DnSysWin32_LogLastError();
  }
}

void* DnMemVirtual_Commit(void* page, u64 size) {
  DN_ASSERT(DN_MEM_IS_ALIGNED(size, DnMem_SystemPageSize));

  DWORD flags = MEM_COMMIT;
  if (page == nullptr) {
    flags |= MEM_RESERVE;
  }

  void* address = VirtualAlloc(page, size, flags, PAGE_READWRITE);
  if (DN_UNLIKELY(address == nullptr)) {
    DN_LOG_ERROR("Failed to commit virtual memory");
    DnSysWin32_LogLastError();
  }

  return address;
}
