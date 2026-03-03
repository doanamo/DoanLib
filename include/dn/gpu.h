#pragma once

/*
 * GPU device
 */

typedef struct DnGpuDevice DnGpuDevice;

DnGpuDevice* DnGpuDevice_Create();
void DnGpuDevice_Destroy(DnGpuDevice* device);

/*
 * GPU swap chain
 */

typedef struct DnGpuSwapChain DnGpuSwapChain;

DnGpuSwapChain* DnGpuSwapChain_Create();
void DnGpuSwapChain_Present(DnGpuSwapChain* swapChain);
void DnGpuSwapChain_Destroy(DnGpuSwapChain* swapChain);
