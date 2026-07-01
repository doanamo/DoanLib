#include "dn/memory.h"

// == LARGE MEMORY ========================================================== //

static void* DnMemLarge_Alloc(const DnMemAllocator* allocator, u64 size, u64 alignment) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);
  DN_UNUSED(alignment);

  void* allocation = DnMemVirtual_Commit(nullptr, size);
  DN_ASSERT_ALWAYS(allocation != nullptr);
  DN_ASSERT(DN_MEM_IS_ALIGNED((u64)allocation, DnMem_SystemPageSize));

  return allocation;
}

static void DnMemLarge_Free(const DnMemAllocator* allocator, void* allocation) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  if (allocation) {
    DnMemVirtual_Release(allocation);
  }
}

static void* DnMemLarge_Realloc(const DnMemAllocator* allocator, void* allocation, u64 size, u64 alignment) {
  DN_ASSERT(allocator);
  DN_UNUSED(allocator);

  if (size == 0) {
    DnMemLarge_Free(allocator, allocation);
    return nullptr;
  }

  void* reallocation = DnMemLarge_Alloc(allocator, size, alignment);
  DN_ASSERT_ALWAYS(reallocation);

  if (allocation) {
    u64 oldSize = DnMemVirtual_QuerySize(allocation);
    DN_ASSERT(oldSize > 0);

    memcpy(reallocation, allocation, DN_MIN(oldSize, size));
    DnMemLarge_Free(allocator, allocation);
  }

  return reallocation;
}

static DnMemAllocator g_dnMemLargeAllocator = {
  .alloc = &DnMemLarge_Alloc,
  .realloc = &DnMemLarge_Realloc,
  .free = &DnMemLarge_Free,
  .context = nullptr,
};

const DnMemAllocator* DnMemLarge_GetAllocator() {
  return &g_dnMemLargeAllocator;
}
