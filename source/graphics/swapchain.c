#include "shared.h"
#include "swapchain.h"
#include "device.h"
#include "platform/window.h"

IDXGISwapChain4* g_swapchain = nullptr;

bool graphics_swapchain_init() {
  LOG_INFO("Initializing graphics swapchain");
  bool result = false;

  IDXGISwapChain1* base_swapchain = nullptr;

  DXGI_SWAP_CHAIN_DESC1 swapchain_desc = {
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

  if (FAILED(IDXGIFactory7_CreateSwapChainForHwnd(g_graphics_dxgi_factory, (IUnknown*)g_graphics_d3d11_device, g_platform_window_handle, &swapchain_desc,nullptr, nullptr, &base_swapchain))) {
    LOG_ERROR("Failed to create DXGI swapchain");
    goto error;
  }

  if (FAILED(IDXGISwapChain1_QueryInterface(base_swapchain, &IID_IDXGISwapChain4, (void**)&g_swapchain))) {
    LOG_ERROR("Failed to query DXGI swapchain interface");
    goto error;
  }

  result = true;

error:
  if (base_swapchain) {
    IDXGISwapChain1_Release(base_swapchain);
  }

  return result;
}

void graphics_swapchain_present() {
  IDXGISwapChain4_Present(g_swapchain, 1, 0);
}

void graphics_swapchain_deinit() {
  LOG_INFO("Deinitializing graphics swapchain");

  if (g_swapchain) {
    IDXGISwapChain4_Release(g_swapchain);
    g_swapchain = nullptr;
  }
}
