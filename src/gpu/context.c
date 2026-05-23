#include "dn/gpu.h"
#include "dn/memory.h"

struct DnGpuContext {
  VkInstance instance;
};

static void DnGpuContext_PrintAvailableInstanceLayers() {
  bool success = false;

  DnMemTempScope tempScope = DnMemTemp_PushScope();

  u32 availableLayerCount = 0;
  if (vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr) != VK_SUCCESS) {
    goto error;
  }

  VkLayerProperties* availableLayers = DN_MEM_ALLOC_TYPES(g_dnMemAllocatorTemp, VkLayerProperties, availableLayerCount);
  if (vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers) != VK_SUCCESS) {
    goto error;
  }

  DN_LOG_INFO("Available Vulkan instance layers:");
  for (u32 i = 0; i < availableLayerCount; i++) {
    DN_LOG_INFO("  %s", availableLayers[i].layerName);
  }

  success = true;

error:
  if (!success) {
    DN_LOG_ERROR("Failed to enumerate available instance layers");
  }

  DnMemTemp_PopScope(&tempScope);
}

static void DnGpuContext_PrintAvailableInstanceExtensions() {
  bool success = false;

  DnMemTempScope tempScope = DnMemTemp_PushScope();

  u32 availableExtensionCount = 0;
  if (vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr) != VK_SUCCESS) {
    goto error;
  }

  VkExtensionProperties* availableExtensions = DN_MEM_ALLOC_TYPES(g_dnMemAllocatorTemp, VkExtensionProperties, availableExtensionCount);
  if (vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions) != VK_SUCCESS) {
    goto error;
  }

  DN_LOG_INFO("Available Vulkan instance extensions:");
  for (u32 i = 0; i < availableExtensionCount; i++) {
    DN_LOG_INFO("  %s", availableExtensions[i].extensionName);
  }

  success = true;

error:
  if (!success) {
    DN_LOG_ERROR("Failed to enumerate available instance extensions");
  }

  DnMemTemp_PopScope(&tempScope);
}

DnGpuContext* DnGpuContext_Create() {
  DN_LOG_INFO("Creating gpu context");
  bool success = false;

  DnGpuContext* context = DN_MEM_ALLOC_TYPE(g_dnMemAllocatorDefault, DnGpuContext);
  *context = (DnGpuContext) {};

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

  DnGpuContext_PrintAvailableInstanceLayers();
  DnGpuContext_PrintAvailableInstanceExtensions();

  const char* enabledExtensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
  };

  VkInstanceCreateInfo createInfo = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &appInfo,
    .enabledLayerCount = 0,
    .ppEnabledLayerNames = nullptr,
    .enabledExtensionCount = DN_ARRAY_LENGTH(enabledExtensions),
    .ppEnabledExtensionNames = enabledExtensions,
  };

  if (vkCreateInstance(&createInfo, g_dnGpuAllocatorVulkan, &context->instance) != VK_SUCCESS) {
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

  vkDestroyInstance(context->instance, g_dnGpuAllocatorVulkan);

  DN_MEM_FREE(g_dnMemAllocatorDefault, context);
}
