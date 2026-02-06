#include "shared.h"
#include "swapchain.h"
#include "device.h"
#include "sys/window.h"

IDXGISwapChain4* g_gpuSwapchainDXGI = nullptr;

bool GpuSwapchainInit() {
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

  if (FAILED(IDXGIFactory7_CreateSwapChainForHwnd(g_gpuFactoryDXGI, (IUnknown*)g_gpuDeviceD3D11, g_sysWindowHandle, &swapchainDesc,nullptr, nullptr, &baseSwapchainDXGI))) {
    LOG_ERROR("Failed to create DXGI swapchain");
    goto error;
  }

  if (FAILED(IDXGISwapChain1_QueryInterface(baseSwapchainDXGI, &IID_IDXGISwapChain4, (void**)&g_gpuSwapchainDXGI))) {
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

void GpuSwapchainPresent() {
  IDXGISwapChain4_Present(g_gpuSwapchainDXGI, 1, 0);
}

void GpuSwapchainDeinit() {
  LOG_INFO("Deinitializing gpu swapchain");

  if (g_gpuSwapchainDXGI) {
    IDXGISwapChain4_Release(g_gpuSwapchainDXGI);
    g_gpuSwapchainDXGI = nullptr;
  }
}
