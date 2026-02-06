#include "shared.h"
#include "swapchain.h"
#include "device.h"
#include "sys/window.h"

IDXGISwapChain4* g_dnGpuSwapchainDXGI = nullptr;

bool DnGpuSwapchainInit() {
  LOG_INFO("Initializing gpu swapchain");
  bool result = false;

  IDXGISwapChain1* baseSwapchainDXGI = nullptr;

  DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {
    .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
    .SampleDesc = {
      .Count = 1,
      .Quality = 0,
    },
    .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
    .BufferCount = 2,
    .Scaling = DXGI_SCALING_STRETCH,
    .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
    .AlphaMode = DXGI_ALPHA_MODE_IGNORE,
    .Flags = 0,
  };

  if (FAILED(IDXGIFactory7_CreateSwapChainForHwnd(g_dnGpuFactoryDXGI, (IUnknown*)g_dnGpuDeviceD3D11, g_dnSysWindowHandle, &swapchainDesc,nullptr, nullptr, &baseSwapchainDXGI))) {
    LOG_ERROR("Failed to create DXGI swapchain");
    goto error;
  }

  if (FAILED(IDXGISwapChain1_QueryInterface(baseSwapchainDXGI, &IID_IDXGISwapChain4, (void**)&g_dnGpuSwapchainDXGI))) {
    LOG_ERROR("Failed to query DXGI swapchain interface");
    goto error;
  }

  result = true;

error:
  if (baseSwapchainDXGI) {
    IDXGISwapChain1_Release(baseSwapchainDXGI);
  }

  return result;
}

void DnGpuSwapchainPresent() {
  IDXGISwapChain4_Present(g_dnGpuSwapchainDXGI, 1, 0);
}

void DnGpuSwapchainDeinit() {
  LOG_INFO("Deinitializing gpu swapchain");

  if (g_dnGpuSwapchainDXGI) {
    IDXGISwapChain4_Release(g_dnGpuSwapchainDXGI);
    g_dnGpuSwapchainDXGI = nullptr;
  }
}
