#include "shared.h"
#include "device.h"

ID3D11Device5* g_graphics_device = nullptr;
ID3D11DeviceContext4* g_graphics_context = nullptr;

bool graphics_device_init() {
  bool result = false;

  LOG_INFO("Initializing graphics device");

  UINT device_flags = 0;
  D3D_FEATURE_LEVEL feature_levels[] = {
    D3D_FEATURE_LEVEL_10_1,
  };

  ID3D11Device* base_device;
  ID3D11DeviceContext* base_context;
  if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, device_flags, feature_levels, ARRAY_LENGTH(feature_levels), D3D11_SDK_VERSION, &base_device, nullptr, &base_context))) {
    LOG_ERROR("Failed to crate graphics device");
    goto error;
  }

  if (FAILED(ID3D11Device_QueryInterface(base_device, &IID_ID3D11Device5, (void**)&g_graphics_device))) {
    LOG_ERROR("Failed to query graphics device interface");
    goto error;
  }

  if (FAILED(ID3D11DeviceContext_QueryInterface(base_context, &IID_ID3D11DeviceContext4, (void**)&g_graphics_context))) {
    LOG_ERROR("Failed to query device context interface");
    goto error;
  }

  result = true;

error:
  if (base_context) {
    ID3D11DeviceContext_Release(base_context);
  }

  if (base_device) {
    ID3D11Device_Release(base_device);
  }

  return result;
}

void graphics_device_deinit() {
  LOG_INFO("Deinitializing graphics device");

  if (g_graphics_context) {
    ID3D11DeviceContext4_Release(g_graphics_context);
    g_graphics_context = nullptr;
  }

  if (g_graphics_context) {
    ID3D11Device5_Release(g_graphics_device);
    g_graphics_device = nullptr;
  }
}
