#pragma once

extern IDXGISwapChain4* g_graphics_dxgi_swapchain;

bool graphics_swapchain_init();
void graphics_swapchain_present();
void graphics_swapchain_deinit();
