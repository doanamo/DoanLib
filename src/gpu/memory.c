#include "dn/gpu.h"
#include "dn/memory.h"

// == VULKAN MEMORY ALLOCATOR =============================================== //

static void* VKAPI_PTR DnGpuMemoryVulkan_Alloc(void* userData, u64 size, u64 alignment, VkSystemAllocationScope scope) {
  DN_UNUSED(userData);
  DN_UNUSED(scope);

  return DN_MEM_ALLOC(DnMemAllocator_GetDefault(), size, alignment);
}

static void* VKAPI_PTR DnGpuMemoryVulkan_Realloc(void* userData, void* allocation, u64 size, u64 alignment, VkSystemAllocationScope scope) {
  DN_UNUSED(userData);
  DN_UNUSED(scope);

  return DN_MEM_REALLOC(DnMemAllocator_GetDefault(), allocation, size, alignment);
}

static void VKAPI_PTR DnGpuMemoryVulkan_Free(void* userData, void* allocation) {
  DN_UNUSED(userData);

  DN_MEM_FREE(DnMemAllocator_GetDefault(), allocation);
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
