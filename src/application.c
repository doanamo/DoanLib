#include "dn/application.h"
#include "dn/system.h"
#include "dn/gpu.h"

DnSysWindow* g_sysWindow = nullptr;
DnGpuDevice* g_gpuDevice = nullptr;
DnGpuSwapChain* g_gpuSwapChain = nullptr;

bool g_exit = false;
int g_exitCode = 1;

void DnApp_CloseCallback() {
  g_exit = true;
}

bool DnApp_Init() {
  DN_LOG_INFO("Initializing application");

  if (!DnMemAllocators_Init()) {
    return false;
  }

  g_sysWindow = DnSysWindow_Create();
  if (!g_sysWindow) {
    return false;
  }

  DnSysWindow_SetSize(g_sysWindow, 1024, 576);
  DnSysWindow_SetTitle(g_sysWindow, "DoanLib Example");
  DnSysWindow_SetCloseCallback(g_sysWindow, &DnApp_CloseCallback);

  g_gpuDevice = DnGpuDevice_Create();
  if (!g_gpuDevice) {
    return false;
  }

  g_gpuSwapChain = DnGpuSwapChain_Create();
  if (!g_gpuSwapChain) {
    return false;
  }

  return true;
}

void DnApp_Update(float deltaTime) {
  DN_UNUSED(deltaTime);
}

void DnApp_Render(float alphaTime) {
  DN_UNUSED(alphaTime);
}

void DnApp_Deinit() {
  DN_LOG_INFO("Deinitializing application");

  if (g_gpuSwapChain) {
    DnGpuSwapChain_Destroy(g_gpuSwapChain);
    g_gpuSwapChain = nullptr;
  }

  if (g_gpuDevice) {
    DnGpuDevice_Destroy(g_gpuDevice);
    g_gpuDevice = nullptr;
  }

  if (g_sysWindow) {
    DnSysWindow_Destroy(g_sysWindow);
    g_sysWindow = nullptr;
  }

  DnMemAllocators_Deinit();
}

int DnApp_Run() {
  if (!DnApp_Init()) {
    goto error;
  }

  DN_LOG_INFO("Running application");
  DnSysWindow_SetVisibility(g_sysWindow, true);

  while (!g_exit) {
    DnSysWindow_ProcessMessages(g_sysWindow);
    DnApp_Update(0.0f);
    DnApp_Render(1.0f);
    DnGpuSwapChain_Present(g_gpuSwapChain);
  }

  g_exitCode = 0;

error:
  DnApp_Deinit();
  return g_exitCode;
}
