#include "dn/application.h"
#include "dn/system.h"
#include "dn/gpu.h"

DnSysWindow* g_window = nullptr;

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

  g_window = DnSysWindow_Create();
  if (!g_window) {
    return false;
  }

  DnSysWindow_SetSize(g_window, 1024, 576);
  DnSysWindow_SetTitle(g_window, "DoanLib Example");
  DnSysWindow_SetCloseCallback(g_window, &DnApp_CloseCallback);

  if (!DnGpuDevice_Init()) {
    return false;
  }

  if (!DnGpuSwapChain_Init()) {
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
  DnGpuSwapChain_Deinit();
  DnGpuDevice_Deinit();

  if (g_window) {
    DnSysWindow_Destroy(g_window);
    g_window = nullptr;
  }

  DnMemAllocators_Deinit();
}

int DnApp_Run() {
  if (!DnApp_Init()) {
    goto error;
  }

  DN_LOG_INFO("Running application");
  DnSysWindow_SetVisibility(g_window, true);

  while (!g_exit) {
    DnSysWindow_ProcessMessages(g_window);
    DnApp_Update(0.0f);
    DnApp_Render(1.0f);
    DnGpuSwapChain_Present();
  }

  g_exitCode = 0;

error:
  DnApp_Deinit();
  return g_exitCode;
}
