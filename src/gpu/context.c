#include "dn/gpu.h"
#include "dn/memory.h"
#include "vulkan/vulkan_core.h"

struct DnGpuContext {
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkPhysicalDevice physicalDevice;
  VkDevice device;
  VkQueue graphicsQueue;
};

#ifdef DN_LOG_ENABLED

static void DnGpuContext_PrintAvailableInstanceLayers() {
  DnMemTempScope tempScope = DnMemTemp_PushScope();

  u32 availableLayerCount = 0;
  if (vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr) != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to enumerate Vulkan instance layer count");
    goto error;
  }

  VkLayerProperties* availableLayers = DN_MEM_ALLOC_TYPES(g_dnMemAllocatorTemp, VkLayerProperties, availableLayerCount);
  if (vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers) != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to enumerate Vulkan instance layers");
    goto error;
  }

  DN_LOG_INFO("Available Vulkan instance layers:");
  for (u32 i = 0; i < availableLayerCount; i++) {
    DN_LOG_INFO("  %s", availableLayers[i].layerName);
  }

error:
  DnMemTemp_PopScope(&tempScope);
}

static void DnGpuContext_PrintAvailableInstanceExtensions() {
  DnMemTempScope tempScope = DnMemTemp_PushScope();

  u32 availableExtensionCount = 0;
  if (vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr) != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to enumerate Vulkan instance extension count");
    goto error;
  }

  VkExtensionProperties* availableExtensions = DN_MEM_ALLOC_TYPES(g_dnMemAllocatorTemp, VkExtensionProperties, availableExtensionCount);
  if (vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions) != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to enumerate Vulkan instance extensions");
    goto error;
  }

  DN_LOG_INFO("Available Vulkan instance extensions:");
  for (u32 i = 0; i < availableExtensionCount; i++) {
    DN_LOG_INFO("  %s", availableExtensions[i].extensionName);
  }

error:
  DnMemTemp_PopScope(&tempScope);
}

static void DnGpuContext_PrintAvailableDeviceExtensions(VkPhysicalDevice physicalDevice) {
  DnMemTempScope tempScope = DnMemTemp_PushScope();

  u32 availableExtensionCount = 0;
  if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &availableExtensionCount, nullptr) != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to enumerate Vulkan device extension count");
    goto error;
  }

  VkExtensionProperties* availableExtensions = DN_MEM_ALLOC_TYPES(g_dnMemAllocatorTemp, VkExtensionProperties, availableExtensionCount);
  if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &availableExtensionCount, availableExtensions) != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to enumerate Vulkan device extensions");
    goto error;
  }

  DN_LOG_INFO("Available Vulkan device extensions:");
  for (u32 i = 0; i < availableExtensionCount; i++) {
    DN_LOG_INFO("  %s", availableExtensions[i].extensionName);
  }

error:
  DnMemTemp_PopScope(&tempScope);
}

#endif

static bool DnGpuContext_CreateInstance(DnGpuContext* context) {
  DN_LOG_INFO("Creating Vulkan instance");
  bool success = false;

  VkApplicationInfo appInfo = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .apiVersion = VK_API_VERSION_1_4,
    .pEngineName = "DoanLib",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    .pApplicationName = nullptr,
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
  };

#ifdef DN_LOG_ENABLED
  DnGpuContext_PrintAvailableInstanceLayers();
  DnGpuContext_PrintAvailableInstanceExtensions();
#endif

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
    DN_LOG_ERROR("Failed to create Vulkan instance");
    goto error;
  }

  volkLoadInstance(context->instance);

  success = true;

error:
  return success;
}

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

static bool DnGpuContext_CreateDebugMessenger(DnGpuContext* context) {
  bool success = false;

  VkDebugUtilsMessengerCreateInfoEXT debugMessangerCreateInfo = {
    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
    .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    .pfnUserCallback = DnGpuContext_DebugCallback,
  };

  if (vkCreateDebugUtilsMessengerEXT(context->instance, &debugMessangerCreateInfo, g_dnGpuAllocatorVulkan, &context->debugMessenger)) {
    DN_LOG_ERROR("Failed to create Vulkan debug messenger");
    goto error;
  }

  success = true;

error:
  return success;
}

#endif

static bool DnGpuContext_SelectPhysicalDevice(DnGpuContext* context) {
  bool success = false;

  DnMemTempScope tempScope = DnMemTemp_PushScope();

  u32 physicalDeviceCount;
  if (vkEnumeratePhysicalDevices(context->instance, &physicalDeviceCount, nullptr) != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to enumerate Vulkan physical device count");
    goto error;
  }

  if (physicalDeviceCount == 0) {
    DN_LOG_ERROR("No available Vulkan physical devices found");
    goto error;
  }

  VkPhysicalDevice* physicalDevices = DN_MEM_ALLOC_TYPES(g_dnMemAllocatorTemp, VkPhysicalDevice, physicalDeviceCount);
  if (vkEnumeratePhysicalDevices(context->instance, &physicalDeviceCount, physicalDevices) != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to enumerate Vulkan physical devices");
    goto error;
  }

  for (u32 i = 0; i < physicalDeviceCount; i++) {
    VkPhysicalDevice physicalDevice = physicalDevices[i];
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

    if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      context->physicalDevice = physicalDevice;
      break;
    }
  }

  if (context->physicalDevice == nullptr) {
    context->physicalDevice = physicalDevices[0];
  }

#ifdef DN_LOG_ENABLED
  DN_LOG_INFO("Available Vulkan physical devices:");
  for (u32 i = 0; i < physicalDeviceCount; i++) {
    VkPhysicalDevice physicalDevice = physicalDevices[i];
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

    const char* selectedText = (physicalDevice == context->physicalDevice) ? " (Selected)" : "";
    DN_LOG_INFO("  %s%s", physicalDeviceProperties.deviceName, selectedText);
  }
#endif

  success = true;

error:
  DnMemTemp_PopScope(&tempScope);

  return success;
}

static bool DnGpuContext_CreateDevice(DnGpuContext* context) {
  DN_LOG_INFO("Creating Vulkan device");
  bool success = false;

  DnMemTempScope tempScope = DnMemTemp_PushScope();

  u32 queueFamilyCount;
  vkGetPhysicalDeviceQueueFamilyProperties(context->physicalDevice, &queueFamilyCount, nullptr);

  if (queueFamilyCount == 0) {
    DN_LOG_ERROR("No available Vulkan queue families found");
    goto error;
  }

  VkQueueFamilyProperties* queueFamilyProperties = DN_MEM_ALLOC_TYPES(g_dnMemAllocatorTemp, VkQueueFamilyProperties, queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(context->physicalDevice, &queueFamilyCount, queueFamilyProperties);

  u32 queueFamilyIndex;
  bool queueFamilyFound = false;
  for (u32 i = 0; i < queueFamilyCount; i++) {
    if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      queueFamilyIndex = i;
      queueFamilyFound = true;
      break;
    }
  }

  if (!queueFamilyFound) {
    DN_LOG_ERROR("Failed to find suitable Vulkan queue family");
    goto error;
  }

  const float queuePriorities[] = { 1.0f };
  VkDeviceQueueCreateInfo queueCreateInfo = {};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = queuePriorities;

#ifdef DN_LOG_ENABLED
  DnGpuContext_PrintAvailableDeviceExtensions(context->physicalDevice);
#endif

  const char* requiredDeviceExtensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  };

  VkDeviceCreateInfo deviceCreateInfo = {};
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.queueCreateInfoCount = 1;
  deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
  deviceCreateInfo.enabledExtensionCount = DN_ARRAY_LENGTH(requiredDeviceExtensions);
  deviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExtensions;

  if (vkCreateDevice(context->physicalDevice, &deviceCreateInfo, g_dnGpuAllocatorVulkan, &context->device) != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to create Vulkan device");
    goto error;
  }

  vkGetDeviceQueue(context->device, queueFamilyIndex, 0, &context->graphicsQueue);
  volkLoadDevice(context->device);

  success = true;

error:
  DnMemTemp_PopScope(&tempScope);

  return success;
}

DnGpuContext* DnGpuContext_Create() {
  DN_LOG_INFO("Creating gpu context");
  bool success = false;

  DnGpuContext* context = DN_MEM_ALLOC_TYPE(g_dnMemAllocatorDefault, DnGpuContext);
  *context = (DnGpuContext) {};

  if (volkInitialize() != VK_SUCCESS) {
    DN_LOG_ERROR("Failed to initialize Volk");
    goto error;
  }

  if (!DnGpuContext_CreateInstance(context)) {
    goto error;
  }

#ifdef DN_CONFIG_DEBUG
  if (!DnGpuContext_CreateDebugMessenger(context)) {
    goto error;
  }
#endif

  if (!DnGpuContext_SelectPhysicalDevice(context)) {
    goto error;
  }

  if (!DnGpuContext_CreateDevice(context)) {
    goto error;
  }

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

  if (context->device) {
    vkDestroyDevice(context->device, g_dnGpuAllocatorVulkan);
  }

  if (context->debugMessenger) {
    vkDestroyDebugUtilsMessengerEXT(context->instance, context->debugMessenger, g_dnGpuAllocatorVulkan);
  }

  if (context->instance) {
    vkDestroyInstance(context->instance, g_dnGpuAllocatorVulkan);
  }

  DN_MEM_FREE(g_dnMemAllocatorDefault, context);
}
