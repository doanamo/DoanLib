#pragma once

extern IDXGISwapChain4* g_dnGpuSwapChain;
extern ID3D11Texture2D1* g_dnGpuFrameBuffer;
extern ID3D11Texture2D1* g_dnGpuDepthBuffer;
extern ID3D11RenderTargetView1* g_dnGpuFrameBufferView;
extern ID3D11DepthStencilView* g_dnGpuDepthBufferView;

bool DnGpuSwapChainInit();
void DnGpuSwapChainPresent();
void DnGpuSwapChainDeinit();
