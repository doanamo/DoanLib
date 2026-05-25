#include "dn/gpu.h"
#include "dn/memory.h"
#include "dn/system.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct DnGpuSwapChain {
  DnGpuContext* context;
  VkSurfaceKHR surface;
};

bool DnGpuSwapChain_CreateSurface(DnGpuContext* context, DnSysWindow* window, DnGpuSwapChain* swapChain) {
  DN_LOG_INFO("Creating gpu surface");
  bool success = false;

  VkWin32SurfaceCreateInfoKHR surfaceInfo = {
    .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
    .hinstance = GetModuleHandle(nullptr),
    .hwnd = (HWND)DnSysWindow_GetPrivateHandle(window),
  };

  if (vkCreateWin32SurfaceKHR(DnGpuContext_GetInstance(context), &surfaceInfo, nullptr, &swapChain->surface) != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to create Vulkan surface");
    goto error;
  }

  success = true;

error:
  return success;
}

DnGpuSwapChain* DnGpuSwapChain_Create(DnGpuContext* context, DnSysWindow* window) {
  DN_ASSERT(context);
  DN_ASSERT(window);

  DN_LOG_INFO("Creating gpu swapchain");
  bool success = false;

  DnGpuSwapChain* swapChain = DN_MEM_ALLOC_TYPE(g_dnMemAllocatorDefault, DnGpuSwapChain);
  *swapChain = (DnGpuSwapChain) {
    .context = context,
  };

  if (!DnGpuSwapChain_CreateSurface(context, window, swapChain)) {
    goto error;
  }

  success = true;

error:
  if (!success) {
    DnGpuSwapChain_Destroy(swapChain);
    swapChain = nullptr;
  }

  return swapChain;
}

void DnGpuSwapChain_Present(DnGpuSwapChain* swapChain) {
  DN_ASSERT(swapChain);
  DN_UNUSED(swapChain);
}

void DnGpuSwapChain_Destroy(DnGpuSwapChain* swapChain) {
  DN_ASSERT(swapChain);

  if (swapChain->surface) {
    vkDestroySurfaceKHR(DnGpuContext_GetInstance(swapChain->context), swapChain->surface, nullptr);
  }

  DN_MEM_FREE(g_dnMemAllocatorDefault, swapChain);
}
