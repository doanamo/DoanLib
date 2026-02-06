#pragma once

extern IDXGISwapChain4* g_dnGpuSwapchain;
extern ID3D11Texture2D1* g_dnGpuFramebuffer;

bool DnGpuSwapchainInit();
void DnGpuSwapchainPresent();
void DnGpuSwapchainDeinit();
