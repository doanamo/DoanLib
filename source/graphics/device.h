#pragma once

extern ID3D11Device5* g_gpuDeviceD3D11;
extern ID3D11DeviceContext4* g_gpuContextD3D11;

extern IDXGIDevice4* g_gpuDeviceDXGI;
extern IDXGIAdapter4* _gpuAdapterDXGI;
extern IDXGIFactory7* g_gpuFactoryDXGI;

bool GpuDeviceInit();
void GpuDeviceDeinit();
