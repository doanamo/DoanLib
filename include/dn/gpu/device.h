#pragma once

extern ID3D11Device5* g_dnGpuDevice;
extern ID3D11DeviceContext4* g_dnGpuContext;

extern IDXGIDevice4* g_dnGpuDeviceDXGI;
extern IDXGIAdapter4* g_dnGpuAdapterDXGI;
extern IDXGIFactory7* g_dnGpuFactoryDXGI;

bool DnGpuDeviceInit();
void DnGpuDeviceDeinit();
