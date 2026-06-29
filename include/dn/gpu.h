#pragma once

#include <Volk/volk.h>

// == GPU DEFINES =========================================================== //

// Defines whether GPU validation is enabled. This greatly reduces graphics
// performance but is useful for debugging.
#define DN_GPU_VALIDATION_ENABLED DN_CONFIG_DEBUG

// == GPU MEMORY ============================================================ //

// System allocator for use with Vulkan functions.
extern const VkAllocationCallbacks* const g_dnGpuAllocatorVulkan;

// == GPU CONTEXT =========================================================== //

// Opaque handle for GPU context.
typedef struct DnGpuContext DnGpuContext;

// Creates GPU context using Vulkan.
DnGpuContext* DnGpuContext_Create();

// Destroys GPU context.
void DnGpuContext_Destroy(DnGpuContext* context);

// Returns Vulkan instance associated with GPU context.
VkInstance DnGpuContext_GetInstance(DnGpuContext* context);

// Returns Vulkan device associated with GPU context.
VkDevice DnGpuContext_GetDevice(DnGpuContext* context);

// == GPU SWAPCHAIN ========================================================= //

typedef struct DnSysWindow DnSysWindow;

// Opaque handle for GPU swap chain.
typedef struct DnGpuSwapChain DnGpuSwapChain;

// Creates GPU swap chain and associated it with given window where it will be
// presented. Size of the swap chain is determined by the window size.
DnGpuSwapChain* DnGpuSwapChain_Create(DnGpuContext* context, DnSysWindow* window);

// Destroys GPU swap chain.
void DnGpuSwapChain_Destroy(DnGpuSwapChain* swapChain);

// Presents GPU swap chain to the window it was associated with.
void DnGpuSwapChain_Present(DnGpuSwapChain* swapChain);
