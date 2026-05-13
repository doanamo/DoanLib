#include "dn/gpu.h"
#include "dn/memory.h"

#include <vma/vk_mem_alloc.h>

struct DnGpuContext {
};

DnGpuContext* DnGpuContext_Create() {
  DN_LOG_INFO("Creating gpu context");

  if (volkInitialize() != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to initialize volk");
    return nullptr;
  }

  DnGpuContext* context = DN_MEM_ALLOC_TYPE(g_dnMemAllocatorDefault, DnGpuContext);
  *context = (DnGpuContext){};

  return context;
}

void DnGpuContext_Destroy(DnGpuContext* context) {
  DN_ASSERT(context);
  DN_MEM_FREE(g_dnMemAllocatorDefault, context);
}
