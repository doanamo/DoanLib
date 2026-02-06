#include "dn/shared.h"
#include "dn/gpu/swapchain.h"
#include "dn/gpu/device.h"
#include "dn/sys/window.h"

IDXGISwapChain4* g_dnGpuSwapChain = nullptr;
ID3D11Texture2D1* g_dnGpuFrameBuffer = nullptr;
ID3D11Texture2D1* g_dnGpuDepthBuffer = nullptr;
ID3D11RenderTargetView1* g_dnGpuFrameBufferView = nullptr;
ID3D11DepthStencilView* g_dnGpuDepthBufferView = nullptr;

bool DnGpuSwapChainInit() {
  DN_LOG_INFO("Initializing gpu swapchain");
  bool result = false;

  IDXGISwapChain1* baseSwapChain = nullptr;

  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {
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

  if (FAILED(IDXGIFactory7_CreateSwapChainForHwnd(g_dnGpuFactory, (IUnknown*)g_dnGpuDevice, g_dnSysWindowHandle, &swapChainDesc,nullptr, nullptr, &baseSwapChain))) {
    DN_LOG_ERROR("Failed to create DXGI swapchain");
    goto error;
  }

  if (FAILED(IDXGISwapChain1_QueryInterface(baseSwapChain, &IID_IDXGISwapChain4, (void**)&g_dnGpuSwapChain))) {
    DN_LOG_ERROR("Failed to query DXGI swapchain interface");
    goto error;
  }

  if (FAILED(IDXGISwapChain4_GetBuffer(g_dnGpuSwapChain, 0, &IID_ID3D11Texture2D1, (void**)&g_dnGpuFrameBuffer))) {
    DN_LOG_ERROR("Failed to get DXGI swapchain frame buffer");
    goto error;
  }

  D3D11_TEXTURE2D_DESC1 depthBufferDesc;
  ID3D11Texture2D1_GetDesc1(g_dnGpuFrameBuffer, &depthBufferDesc);
  depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

  if (FAILED(ID3D11Device5_CreateTexture2D1(g_dnGpuDevice, &depthBufferDesc, nullptr, &g_dnGpuDepthBuffer))) {
    DN_LOG_ERROR("Failed to create D3D11 swapchain depth buffer");
    goto error;
  }

  D3D11_RENDER_TARGET_VIEW_DESC1 frameBufferViewDesc = {
    .Format = swapChainDesc.Format,
    .ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D,
  };

  if (FAILED(ID3D11Device5_CreateRenderTargetView1(g_dnGpuDevice, (ID3D11Resource*)g_dnGpuFrameBuffer, &frameBufferViewDesc, &g_dnGpuFrameBufferView))) {
    DN_LOG_ERROR("Failed to create D3D11 swapchain frame buffer view");
    goto error;
  }

  D3D11_DEPTH_STENCIL_VIEW_DESC depthBufferViewDesc = {
    .Format = depthBufferDesc.Format,
    .ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D,
  };

  if (FAILED(ID3D11Device5_CreateDepthStencilView(g_dnGpuDevice, (ID3D11Resource*)g_dnGpuDepthBuffer, &depthBufferViewDesc, &g_dnGpuDepthBufferView))) {
    DN_LOG_ERROR("Failed to create D3D11 swapchain depth buffer view");
    goto error;
  }

  result = true;

error:
  if (baseSwapChain) {
    IDXGISwapChain1_Release(baseSwapChain);
  }

  return result;
}

void DnGpuSwapChainPresent() {
  IDXGISwapChain4_Present(g_dnGpuSwapChain, 1, 0);
}

void DnGpuSwapChainDeinit() {
  DN_LOG_INFO("Deinitializing gpu swapchain");

  if (g_dnGpuDepthBufferView) {
    ID3D11DepthStencilView_Release(g_dnGpuDepthBufferView);
    g_dnGpuDepthBufferView = nullptr;
  }

  if (g_dnGpuFrameBufferView) {
    ID3D11RenderTargetView1_Release(g_dnGpuFrameBufferView);
    g_dnGpuFrameBufferView = nullptr;
  }

  if (g_dnGpuDepthBuffer) {
    ID3D11Texture2D1_Release(g_dnGpuDepthBuffer);
    g_dnGpuDepthBuffer = nullptr;
  }

  if (g_dnGpuFrameBuffer) {
    ID3D11Texture2D1_Release(g_dnGpuFrameBuffer);
    g_dnGpuFrameBuffer = nullptr;
  }

  if (g_dnGpuSwapChain) {
    IDXGISwapChain4_Release(g_dnGpuSwapChain);
    g_dnGpuSwapChain = nullptr;
  }
}
