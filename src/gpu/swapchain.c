#include "dn/gpu.h"
#include "dn/memory.h"

struct DnGpuSwapChain {
};

DnGpuSwapChain* DnGpuSwapChain_Create() {
  DN_LOG_INFO("Creating gpu swapchain");

  DnGpuSwapChain* swapChain = DN_MEM_ALLOC_TYPE(g_dnMemAllocatorDefault, DnGpuSwapChain);
  *swapChain = (DnGpuSwapChain){};

  return swapChain;
}

void DnGpuSwapChain_Present(DnGpuSwapChain* swapChain) {
  DN_ASSERT(swapChain);
}

void DnGpuSwapChain_Destroy(DnGpuSwapChain* swapChain) {
  DN_ASSERT(swapChain);
  DN_MEM_FREE(g_dnMemAllocatorDefault, swapChain);
}
