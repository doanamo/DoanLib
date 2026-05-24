#include "dn/gpu.h"
#include "dn/memory.h"

static void* VKAPI_PTR DnGpuMemoryVulkan_Alloc(void* userData, u64 size, u64 alignment, VkSystemAllocationScope scope) {
  DN_UNUSED(userData);
  DN_UNUSED(scope);

  return DN_MEM_ALLOC_ALIGNED(g_dnMemAllocatorDefault, size, alignment);
}

static void* VKAPI_PTR DnGpuMemoryVulkan_Realloc(void* userData, void* allocation, u64 size, u64 alignment, VkSystemAllocationScope scope) {
  DN_UNUSED(userData);
  DN_UNUSED(scope);

  return DN_MEM_REALLOC_ALIGNED(g_dnMemAllocatorDefault, allocation, size, alignment);
}

static void VKAPI_PTR DnGpuMemoryVulkan_Free(void* userData, void* allocation) {
  DN_UNUSED(userData);

  DN_MEM_FREE_ALIGNED(g_dnMemAllocatorDefault, allocation);
}

static VkAllocationCallbacks g_dnGpuAllocatorVulkanPrivate = {
  .pUserData = nullptr,
  .pfnAllocation = DnGpuMemoryVulkan_Alloc,
  .pfnReallocation = DnGpuMemoryVulkan_Realloc,
  .pfnFree = DnGpuMemoryVulkan_Free,
  .pfnInternalAllocation = nullptr,
  .pfnInternalFree = nullptr,
};

const VkAllocationCallbacks* const g_dnGpuAllocatorVulkan = &g_dnGpuAllocatorVulkanPrivate;
