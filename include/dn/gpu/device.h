#pragma once

extern ID3D11Device5* g_dnGpuDeviceD3D11;
extern ID3D11DeviceContext4* g_dnGpuContextD3D11;

extern IDXGIDevice4* g_dnGpuDeviceDXGI;
extern IDXGIAdapter4* g_dnGpuAdapterDXGI;
extern IDXGIFactory7* g_dnGpuFactoryDXGI;

bool DnGpuDeviceInit();
void DnGpuDeviceDeinit();
