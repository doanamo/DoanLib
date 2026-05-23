#include "dn/gpu.h"
#include "dn/memory.h"
#include "vulkan/vulkan_core.h"

struct DnGpuContext {
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;
};

#ifdef DN_CONFIG_DEBUG

static VKAPI_ATTR VkBool32 VKAPI_CALL DnGpuContext_DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData) {
  DN_UNUSED(severity);
  DN_UNUSED(userData);

  const char* typeText;
  switch (type) {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
      typeText = "General";
      break;

    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
      typeText = "Validation";
      break;

    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
      typeText = "Performance";
      break;

    default:
      typeText = "Unknown";
      break;
  }

  if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    DN_LOG_ERROR("Vulkan: %s: %s", typeText, callbackData->pMessage);
  }
  else {
    DN_LOG_INFO("Vulkan: %s: %s", typeText, callbackData->pMessage);
  }

  return VK_FALSE;
}

#endif

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

  const char* enabledInstanceLayers[] = {
#ifdef DN_CONFIG_DEBUG
    "VK_LAYER_KHRONOS_validation",
    "VK_LAYER_KHRONOS_synchronization2",
#endif
  };

  const char* enabledInstanceExtensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#ifdef DN_CONFIG_DEBUG
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
  };

  VkInstanceCreateInfo instanceCreateInfo = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &appInfo,
    .enabledLayerCount = DN_ARRAY_LENGTH(enabledInstanceLayers),
    .ppEnabledLayerNames = enabledInstanceLayers,
    .enabledExtensionCount = DN_ARRAY_LENGTH(enabledInstanceExtensions),
    .ppEnabledExtensionNames = enabledInstanceExtensions,
  };

  if (vkCreateInstance(&instanceCreateInfo, g_dnGpuAllocatorVulkan, &context->instance) != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to create instance");
    goto error;
  }

  volkLoadInstance(context->instance);

#ifdef DN_CONFIG_DEBUG
  VkDebugUtilsMessengerCreateInfoEXT debugMessangerCreateInfo = {
    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
    .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    .pfnUserCallback = DnGpuContext_DebugCallback,
  };

  if (vkCreateDebugUtilsMessengerEXT(context->instance, &debugMessangerCreateInfo, g_dnGpuAllocatorVulkan, &context->debugMessenger)) {
    DN_LOG_ERROR("Failed to create debug  messenger");
    goto error;
  }
#endif

  success = true;

error:
  if (!success) {
    DnGpuContext_Destroy(context);
    context = nullptr;
  }

  return context;
}

void DnGpuContext_Destroy(DnGpuContext* context) {
  DN_ASSERT(context);

  if (context->debugMessenger) {
    vkDestroyDebugUtilsMessengerEXT(context->instance, context->debugMessenger, g_dnGpuAllocatorVulkan);
  }

  if (context->instance) {
    vkDestroyInstance(context->instance, g_dnGpuAllocatorVulkan);
  }

  DN_MEM_FREE(g_dnMemAllocatorDefault, context);
}
