#include "shared.h"
#include "device.h"

ID3D11Device5* g_gpuDeviceD3D11 = nullptr;
ID3D11DeviceContext4* g_gpuContextD3D11 = nullptr;

IDXGIDevice4* g_gpuDeviceDXGI = nullptr;
IDXGIAdapter4* g_gpuAdapterDXGI = nullptr;
IDXGIFactory7* g_gpuFactoryDXGI = nullptr;

bool GpuDeviceInit() {
  LOG_INFO("Initializing gpu device");
  bool result = false;

  ID3D11Device* baseDeviceD3D11 = nullptr;
  ID3D11DeviceContext* baseContextD3D11 = nullptr;
  IDXGIAdapter* baseAdapterDXGI = nullptr;

  UINT deviceFlags = 0;
  D3D_FEATURE_LEVEL featureLevels[] = {
    D3D_FEATURE_LEVEL_11_1,
  };

  if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, featureLevels, ARRAY_LENGTH(featureLevels), D3D11_SDK_VERSION, &baseDeviceD3D11, nullptr, &baseContextD3D11))) {
    LOG_ERROR("Failed to create D3D11 device");
    goto error;
  }

  if (FAILED(ID3D11Device_QueryInterface(baseDeviceD3D11, &IID_ID3D11Device5, (void**)&g_gpuDeviceD3D11))) {
    LOG_ERROR("Failed to query D3D111 device interface");
    goto error;
  }

  if (FAILED(ID3D11DeviceContext_QueryInterface(baseContextD3D11, &IID_ID3D11DeviceContext4, (void**)&g_gpuContextD3D11))) {
    LOG_ERROR("Failed to query D3D11 device context interface");
    goto error;
  }

  if (FAILED(ID3D11Device_QueryInterface(baseDeviceD3D11, &IID_IDXGIDevice4, (void**)&g_gpuDeviceDXGI))) {
    LOG_ERROR("Failed to query DXGI device interface");
    goto error;
  }

  if (FAILED(IDXGIDevice4_GetAdapter(g_gpuDeviceDXGI, &baseAdapterDXGI))) {
    LOG_ERROR("Failed to retrieve DXGI adapter");
    goto error;
  }

  if (FAILED(IDXGIAdapter_QueryInterface(baseAdapterDXGI, &IID_IDXGIAdapter4, (void**)&g_gpuAdapterDXGI))) {
    LOG_ERROR("Failed to query DXGI adapter interface");
    goto error;
  }

  if (FAILED(IDXGIAdapter4_GetParent(g_gpuAdapterDXGI, &IID_IDXGIFactory7, (void**)&g_gpuFactoryDXGI))) {
    LOG_ERROR("Failed to retrieve DXGI factory");
    goto error;
  }

  result = true;

error:
  if (baseAdapterDXGI) {
    IDXGIAdapter_Release(baseAdapterDXGI);
  }

  if (baseContextD3D11) {
    ID3D11DeviceContext_Release(baseContextD3D11);
  }

  if (baseDeviceD3D11) {
    ID3D11Device_Release(baseDeviceD3D11);
  }

  return result;
}

void GpuDeviceDeinit() {
  LOG_INFO("Deinitializing gpu device");

  if (g_gpuFactoryDXGI) {
    IDXGIFactory7_Release(g_gpuFactoryDXGI);
    g_gpuFactoryDXGI = nullptr;
  }

  if (g_gpuAdapterDXGI) {
    IDXGIAdapter4_Release(g_gpuAdapterDXGI);
    g_gpuAdapterDXGI = nullptr;
  }

  if (g_gpuDeviceDXGI) {
    IDXGIDevice4_Release(g_gpuDeviceDXGI);
    g_gpuDeviceDXGI = nullptr;
  }

  if (g_gpuContextD3D11) {
    ID3D11DeviceContext4_Release(g_gpuContextD3D11);
    g_gpuContextD3D11 = nullptr;
  }

  if (g_gpuContextD3D11) {
    ID3D11Device5_Release(g_gpuDeviceD3D11);
    g_gpuDeviceD3D11 = nullptr;
  }
}
