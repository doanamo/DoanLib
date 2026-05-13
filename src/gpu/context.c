#include "dn/gpu.h"
#include "dn/memory.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"
#include <vma/vk_mem_alloc.h>
#pragma clang diagnostic pop

struct DnGpuContext {
  VkInstance instance;
};

DnGpuContext* DnGpuContext_Create() {
  DN_LOG_INFO("Creating gpu context");
  bool success = false;

  DnGpuContext* context = DN_MEM_ALLOC_TYPE(g_dnMemAllocatorDefault, DnGpuContext);
  *context = (DnGpuContext){};

  if (volkInitialize() != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to initialize volk");
    goto error;
  }

  VkApplicationInfo appInfo = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .apiVersion = VK_API_VERSION_1_4,
    .pEngineName = "DoanLib",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    .pApplicationName = nullptr,
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
  };

  VkInstanceCreateInfo createInfo = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &appInfo,
    .enabledLayerCount = 0,
    .ppEnabledLayerNames = nullptr,
    .enabledExtensionCount = 0,
    .ppEnabledExtensionNames = nullptr,
  };

  if (vkCreateInstance(&createInfo, nullptr, &context->instance) != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to create instance");
    goto error;
  }

  volkLoadInstance(context->instance);

  success = true;

error:
  if (!success) {
    DN_MEM_FREE(g_dnMemAllocatorDefault, context);
    context = nullptr;
  }

  return context;
}

void DnGpuContext_Destroy(DnGpuContext* context) {
  DN_ASSERT(context);

  vkDestroyInstance(context->instance, nullptr);

  DN_MEM_FREE(g_dnMemAllocatorDefault, context);
}
