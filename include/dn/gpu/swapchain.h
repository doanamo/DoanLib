#pragma once

extern IDXGISwapChain4* g_dnGpuSwapchainDXGI;

bool DnGpuSwapchainInit();
void DnGpuSwapchainPresent();
void DnGpuSwapchainDeinit();
