#include "shared.h"
#include "device.h"

ID3D11Device5* g_graphics_d3d11_device = nullptr;
ID3D11DeviceContext4* g_graphics_d3d11_context = nullptr;

IDXGIDevice4* g_graphics_dxgi_device = nullptr;
IDXGIAdapter4* g_graphics_dxgi_adapter = nullptr;
IDXGIFactory7* g_graphics_dxgi_factory = nullptr;

bool graphics_device_init() {
  LOG_INFO("Initializing graphics device");
  bool result = false;

  ID3D11Device* base_d3d11_device = nullptr;
  ID3D11DeviceContext* base_d3d11_context = nullptr;
  IDXGIAdapter* base_dxgi_adapter = nullptr;

  UINT device_flags = 0;
  D3D_FEATURE_LEVEL feature_levels[] = {
    D3D_FEATURE_LEVEL_11_1,
  };

  if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, device_flags, feature_levels, ARRAY_LENGTH(feature_levels), D3D11_SDK_VERSION, &base_d3d11_device, nullptr, &base_d3d11_context))) {
    LOG_ERROR("Failed to create D3D11 device");
    goto error;
  }

  if (FAILED(ID3D11Device_QueryInterface(base_d3d11_device, &IID_ID3D11Device5, (void**)&g_graphics_d3d11_device))) {
    LOG_ERROR("Failed to query D3D111 device interface");
    goto error;
  }

  if (FAILED(ID3D11DeviceContext_QueryInterface(base_d3d11_context, &IID_ID3D11DeviceContext4, (void**)&g_graphics_d3d11_context))) {
    LOG_ERROR("Failed to query D3D11 device context interface");
    goto error;
  }

  if (FAILED(ID3D11Device_QueryInterface(base_d3d11_device, &IID_IDXGIDevice4, (void**)&g_graphics_dxgi_device))) {
    LOG_ERROR("Failed to query DXGI device interface");
    goto error;
  }

  if (FAILED(IDXGIDevice4_GetAdapter(g_graphics_dxgi_device, &base_dxgi_adapter))) {
    LOG_ERROR("Failed to retrieve DXGI adapter");
    goto error;
  }

  if (FAILED(IDXGIAdapter_QueryInterface(base_dxgi_adapter, &IID_IDXGIAdapter4, (void**)&g_graphics_dxgi_adapter))) {
    LOG_ERROR("Failed to query DXGI adapter interface");
    goto error;
  }

  if (FAILED(IDXGIAdapter4_GetParent(g_graphics_dxgi_adapter, &IID_IDXGIFactory7, (void**)&g_graphics_dxgi_factory))) {
    LOG_ERROR("Failed to retrieve DXGI factory");
    goto error;
  }

  result = true;

error:
  if (base_dxgi_adapter) {
    IDXGIAdapter_Release(base_dxgi_adapter);
  }

  if (base_d3d11_context) {
    ID3D11DeviceContext_Release(base_d3d11_context);
  }

  if (base_d3d11_device) {
    ID3D11Device_Release(base_d3d11_device);
  }

  return result;
}

void graphics_device_deinit() {
  LOG_INFO("Deinitializing graphics device");

  if (g_graphics_dxgi_factory) {
    IDXGIFactory7_Release(g_graphics_dxgi_factory);
    g_graphics_dxgi_factory = nullptr;
  }

  if (g_graphics_dxgi_adapter) {
    IDXGIAdapter4_Release(g_graphics_dxgi_adapter);
    g_graphics_dxgi_adapter = nullptr;
  }

  if (g_graphics_dxgi_device) {
    IDXGIDevice4_Release(g_graphics_dxgi_device);
    g_graphics_dxgi_device = nullptr;
  }

  if (g_graphics_d3d11_context) {
    ID3D11DeviceContext4_Release(g_graphics_d3d11_context);
    g_graphics_d3d11_context = nullptr;
  }

  if (g_graphics_d3d11_context) {
    ID3D11Device5_Release(g_graphics_d3d11_device);
    g_graphics_d3d11_device = nullptr;
  }
}
