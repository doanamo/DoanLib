#include "dn/shared.h"
#include "dn/app.h"
#include "dn/sys/window.h"
#include "dn/gpu/device.h"
#include "dn/gpu/swapchain.h"

bool g_exit = false;
int g_exitCode = 1;

void CloseCallback() {
  g_exit = true;
}

bool DnAppInit() {
  DN_LOG_INFO("Initializing application");

  g_dnSysWindowCloseCallback = &CloseCallback;
  if (!DnSysWindowInit("Example", 1024, 576)) {
    return false;
  }

  if (!DnGpuDeviceInit()) {
    return false;
  }

  if (!DnGpuSwapchainInit()) {
    return false;
  }

  return true;
}

void DnAppUpdate(float deltaTime) {
  DN_UNUSED(deltaTime);
}

void DnAppRender(float alphaTime) {
  DN_UNUSED(alphaTime);
}

void DnAppDeinit() {
  DN_LOG_INFO("Deinitializing application");
  DnGpuSwapchainDeinit();
  DnGpuDeviceDeinit();
  DnSysWindowDeinit();
}

int DnAppRun() {
  if (!DnAppInit()) {
    goto error;
  }

  DN_LOG_INFO("Running application");
  DnSysWindowShow();

  while (!g_exit) {
    DnSysWindowProcessMessages();
    DnAppUpdate(0.0f);
    DnAppRender(1.0f);
    DnGpuSwapchainPresent();
  }

  g_exitCode = 0;

error:
  DnAppDeinit();
  return g_exitCode;
}
