#pragma once

extern ID3D11Device5* g_graphics_d3d11_device;
extern ID3D11DeviceContext4* g_graphics_d3d11_context;

extern IDXGIDevice4* g_graphics_dxgi_device;
extern IDXGIAdapter4* g_graphics_dxgi_adapter;
extern IDXGIFactory7* g_graphics_dxgi_factory;

bool graphics_device_init();
void graphics_device_deinit();
