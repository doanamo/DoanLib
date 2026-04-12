#include "dn/gpu.h"
#include "dn/memory.h"

struct DnGpuDevice {
};

DnGpuDevice* DnGpuDevice_Create() {
  DN_LOG_INFO("Creating gpu device");

  DnGpuDevice* device = DN_MEM_ALLOC_TYPE(DnMemAllocatorDefault_Get(), DnGpuDevice);
  *device = (DnGpuDevice){};

  return device;
}

void DnGpuDevice_Destroy(DnGpuDevice* device) {
  DN_ASSERT(device);
  DN_MEM_FREE(DnMemAllocatorDefault_Get(), device);
}
