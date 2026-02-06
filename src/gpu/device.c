#include "dn/shared.h"
#include "dn/gpu/device.h"

ID3D11Device5* g_dnGpuDevice = nullptr;
ID3D11DeviceContext4* g_dnGpuContext = nullptr;

IDXGIDevice4* g_dnGpuDeviceDXGI = nullptr;
IDXGIAdapter4* g_gpuAdapterDXGI = nullptr;
IDXGIFactory7* g_dnGpuFactoryDXGI = nullptr;

bool DnGpuDeviceInit() {
  DN_LOG_INFO("Initializing gpu device");
  bool result = false;

  ID3D11Device* baseDeviceD3D11 = nullptr;
  ID3D11DeviceContext* baseContextD3D11 = nullptr;
  IDXGIAdapter* baseAdapterDXGI = nullptr;

  UINT deviceFlags = 0;
  D3D_FEATURE_LEVEL featureLevels[] = {
    D3D_FEATURE_LEVEL_11_1,
  };

  if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, featureLevels, DN_ARRAY_LENGTH(featureLevels), D3D11_SDK_VERSION, &baseDeviceD3D11, nullptr, &baseContextD3D11))) {
    DN_LOG_ERROR("Failed to create D3D11 device");
    goto error;
  }

  if (FAILED(ID3D11Device_QueryInterface(baseDeviceD3D11, &IID_ID3D11Device5, (void**)&g_dnGpuDevice))) {
    DN_LOG_ERROR("Failed to query D3D111 device interface");
    goto error;
  }

  if (FAILED(ID3D11DeviceContext_QueryInterface(baseContextD3D11, &IID_ID3D11DeviceContext4, (void**)&g_dnGpuContext))) {
    DN_LOG_ERROR("Failed to query D3D11 device context interface");
    goto error;
  }

  if (FAILED(ID3D11Device_QueryInterface(baseDeviceD3D11, &IID_IDXGIDevice4, (void**)&g_dnGpuDeviceDXGI))) {
    DN_LOG_ERROR("Failed to query DXGI device interface");
    goto error;
  }

  if (FAILED(IDXGIDevice4_GetAdapter(g_dnGpuDeviceDXGI, &baseAdapterDXGI))) {
    DN_LOG_ERROR("Failed to retrieve DXGI adapter");
    goto error;
  }

  if (FAILED(IDXGIAdapter_QueryInterface(baseAdapterDXGI, &IID_IDXGIAdapter4, (void**)&g_gpuAdapterDXGI))) {
    DN_LOG_ERROR("Failed to query DXGI adapter interface");
    goto error;
  }

  if (FAILED(IDXGIAdapter4_GetParent(g_gpuAdapterDXGI, &IID_IDXGIFactory7, (void**)&g_dnGpuFactoryDXGI))) {
    DN_LOG_ERROR("Failed to retrieve DXGI factory");
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

void DnGpuDeviceDeinit() {
  DN_LOG_INFO("Deinitializing gpu device");

  if (g_dnGpuFactoryDXGI) {
    IDXGIFactory7_Release(g_dnGpuFactoryDXGI);
    g_dnGpuFactoryDXGI = nullptr;
  }

  if (g_gpuAdapterDXGI) {
    IDXGIAdapter4_Release(g_gpuAdapterDXGI);
    g_gpuAdapterDXGI = nullptr;
  }

  if (g_dnGpuDeviceDXGI) {
    IDXGIDevice4_Release(g_dnGpuDeviceDXGI);
    g_dnGpuDeviceDXGI = nullptr;
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
