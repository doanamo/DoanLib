#pragma once

#include <Volk/volk.h>

/*
 * GPU context
 */

typedef struct DnGpuContext DnGpuContext;

DnGpuContext* DnGpuContext_Create();
void DnGpuContext_Destroy(DnGpuContext* context);

/*
 * GPU swap chain
 */

typedef struct DnGpuSwapChain DnGpuSwapChain;

DnGpuSwapChain* DnGpuSwapChain_Create();
void DnGpuSwapChain_Present(DnGpuSwapChain* swapChain);
void DnGpuSwapChain_Destroy(DnGpuSwapChain* swapChain);
