#pragma once

/*
 * GPU device
 */

bool DnGpuDevice_Init();
void DnGpuDevice_Deinit();

/*
 * GPU swap chain
 */

bool DnGpuSwapChain_Init();
void DnGpuSwapChain_Deinit();
void DnGpuSwapChain_Present();
