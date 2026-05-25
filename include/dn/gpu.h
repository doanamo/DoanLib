#pragma once

#include <Volk/volk.h>

typedef struct DnSysWindow DnSysWindow;

/*
 * GPU memory
 */

extern const VkAllocationCallbacks* const g_dnGpuAllocatorVulkan;

/*
 * GPU context
 */

typedef struct DnGpuContext DnGpuContext;

DnGpuContext* DnGpuContext_Create();
void DnGpuContext_Destroy(DnGpuContext* context);

VkInstance DnGpuContext_GetInstance(DnGpuContext* context);
VkDevice DnGpuContext_GetDevice(DnGpuContext* context);

/*
 * GPU swap chain
 */

typedef struct DnGpuSwapChain DnGpuSwapChain;

DnGpuSwapChain* DnGpuSwapChain_Create(DnGpuContext* context, DnSysWindow* window);
void DnGpuSwapChain_Present(DnGpuSwapChain* swapChain);
void DnGpuSwapChain_Destroy(DnGpuSwapChain* swapChain);
