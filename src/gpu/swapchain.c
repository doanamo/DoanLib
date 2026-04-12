#include "dn/gpu.h"

struct DnGpuSwapChain {
};

DnGpuSwapChain* DnGpuSwapChain_Create() {
  DN_LOG_INFO("Creating gpu swapchain");

  DnGpuSwapChain* swapChain = DN_MEM_ALLOC_TYPE(DnMemAllocatorDefault_Get(), DnGpuSwapChain);
  *swapChain = (DnGpuSwapChain){};

  return swapChain;
}

void DnGpuSwapChain_Present(DnGpuSwapChain* swapChain) {
  DN_ASSERT(swapChain);
}

void DnGpuSwapChain_Destroy(DnGpuSwapChain* swapChain) {
  DN_ASSERT(swapChain);
  DN_MEM_FREE(DnMemAllocatorDefault_Get(), swapChain);
}
