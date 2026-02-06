#pragma once

extern IDXGISwapChain4* g_gpuSwapchainDXGI;

bool GpuSwapchainInit();
void GpuSwapchainPresent();
void GpuSwapchainDeinit();
