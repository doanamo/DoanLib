#include "dn/shared.h"
#include "dn/gpu/device.h"

ID3D11Device5* g_dnGpuDevice = nullptr;
ID3D11DeviceContext4* g_dnGpuContext = nullptr;
IDXGIDevice4* g_dnGpuDriver = nullptr;
IDXGIAdapter4* g_dnGpuAdapter = nullptr;
IDXGIFactory7* g_dnGpuFactory = nullptr;

bool DnGpuDeviceInit() {
  DN_LOG_INFO("Initializing gpu device");
  bool result = false;

  ID3D11Device* baseDevice = nullptr;
  ID3D11DeviceContext* baseContext = nullptr;
  IDXGIAdapter* baseAdapter = nullptr;

  UINT deviceFlags = 0;
  D3D_FEATURE_LEVEL featureLevels[] = {
    D3D_FEATURE_LEVEL_11_1,
  };

  if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, featureLevels, DN_ARRAY_LENGTH(featureLevels), D3D11_SDK_VERSION, &baseDevice, nullptr, &baseContext))) {
    DN_LOG_ERROR("Failed to create D3D11 device");
    goto error;
  }

  if (FAILED(ID3D11Device_QueryInterface(baseDevice, &IID_ID3D11Device5, (void**)&g_dnGpuDevice))) {
    DN_LOG_ERROR("Failed to query D3D111 device interface");
    goto error;
  }

  if (FAILED(ID3D11DeviceContext_QueryInterface(baseContext, &IID_ID3D11DeviceContext4, (void**)&g_dnGpuContext))) {
    DN_LOG_ERROR("Failed to query D3D11 device context interface");
    goto error;
  }

  if (FAILED(ID3D11Device_QueryInterface(baseDevice, &IID_IDXGIDevice4, (void**)&g_dnGpuDriver))) {
    DN_LOG_ERROR("Failed to query DXGI device interface");
    goto error;
  }

  if (FAILED(IDXGIDevice4_GetAdapter(g_dnGpuDriver, &baseAdapter))) {
    DN_LOG_ERROR("Failed to retrieve DXGI adapter");
    goto error;
  }

  if (FAILED(IDXGIAdapter_QueryInterface(baseAdapter, &IID_IDXGIAdapter4, (void**)&g_dnGpuAdapter))) {
    DN_LOG_ERROR("Failed to query DXGI adapter interface");
    goto error;
  }

  if (FAILED(IDXGIAdapter4_GetParent(g_dnGpuAdapter, &IID_IDXGIFactory7, (void**)&g_dnGpuFactory))) {
    DN_LOG_ERROR("Failed to retrieve DXGI factory");
    goto error;
  }

  result = true;

error:
  if (baseAdapter) {
    IDXGIAdapter_Release(baseAdapter);
  }

  if (baseContext) {
    ID3D11DeviceContext_Release(baseContext);
  }

  if (baseDevice) {
    ID3D11Device_Release(baseDevice);
  }

  return result;
}

void DnGpuDeviceDeinit() {
  DN_LOG_INFO("Deinitializing gpu device");

  if (g_dnGpuFactory) {
    IDXGIFactory7_Release(g_dnGpuFactory);
    g_dnGpuFactory = nullptr;
  }

  if (g_dnGpuAdapter) {
    IDXGIAdapter4_Release(g_dnGpuAdapter);
    g_dnGpuAdapter = nullptr;
  }

  if (g_dnGpuDriver) {
    IDXGIDevice4_Release(g_dnGpuDriver);
    g_dnGpuDriver = nullptr;
  }

  if (g_dnGpuContext) {
    ID3D11DeviceContext4_Release(g_dnGpuContext);
    g_dnGpuContext = nullptr;
  }

  if (g_dnGpuContext) {
    ID3D11Device5_Release(g_dnGpuDevice);
    g_dnGpuDevice = nullptr;
  }
}
