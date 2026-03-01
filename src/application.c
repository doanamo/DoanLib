#include "dn/application.h"
#include "dn/system.h"
#include "dn/gpu.h"

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

  g_dnSysWindowCloseCallback = &DnApp_CloseCallback;
  if (!DnSysWindow_Init("Example", 1024, 576)) {
    return false;
  }

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
  DnSysWindow_Deinit();
  DnMemAllocators_Deinit();
}

int DnApp_Run() {
  if (!DnApp_Init()) {
    goto error;
  }

  DN_LOG_INFO("Running application");
  DnSysWindow_Show();

  while (!g_exit) {
    DnSysWindow_ProcessMessages();
    DnApp_Update(0.0f);
    DnApp_Render(1.0f);
    DnGpuSwapChain_Present();
  }

  g_exitCode = 0;

error:
  DnApp_Deinit();
  return g_exitCode;
}
