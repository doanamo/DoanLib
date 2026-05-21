#include "dn/gpu.h"
#include "dn/memory.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"
#include <vma/vk_mem_alloc.h>
#pragma clang diagnostic pop

static void* VKAPI_PTR DnGpuAllocatorVulkan_Alloc(void* userData, u64 size, u64 alignment, VkSystemAllocationScope scope) {
  DN_UNUSED(userData);
  DN_UNUSED(scope);

  return DN_MEM_ALLOC_ALIGNED(g_dnMemAllocatorDefault, size, alignment);
}

static void* VKAPI_PTR DnGpuAllocatorVulkan_Realloc(void* userData, void* allocation, u64 size, u64 alignment, VkSystemAllocationScope scope) {
  DN_UNUSED(userData);
  DN_UNUSED(scope);

  // #todo: Realloc with proper size argument. Currently fine because it is unused.
  return DN_MEM_REALLOC_ALIGNED(g_dnMemAllocatorDefault, allocation, 0, size, alignment);
}

static void VKAPI_PTR DnGpuAllocatorVulkan_Free(void* userData, void* allocation) {
  DN_UNUSED(userData);

  // #todo: Free with proper size argument. Currently fine because it is unused.
  DN_MEM_FREE_ALIGNED(g_dnMemAllocatorDefault, allocation, 0);
}

static VkAllocationCallbacks g_dnGpuAllocatorVulkanPrivate = {
  .pUserData = nullptr,
  .pfnAllocation = DnGpuAllocatorVulkan_Alloc,
  .pfnReallocation = DnGpuAllocatorVulkan_Realloc,
  .pfnFree = DnGpuAllocatorVulkan_Free,
  .pfnInternalAllocation = nullptr,
  .pfnInternalFree = nullptr,
};

const VkAllocationCallbacks* const g_dnGpuAllocatorVulkan = &g_dnGpuAllocatorVulkanPrivate;
