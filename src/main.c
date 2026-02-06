#include "shared.h"
#include "sys/window.h"
#include "gpu/device.h"
#include "gpu/swapchain.h"
#include "application.h"

bool g_exit = false;
int g_exitCode = 1;

void CloseCallback() {
  g_exit = true;
}

int main(int argc, char* argv[]) {
  UNUSED(argc);
  UNUSED(argv);

  g_sysWindowCloseCallback = &CloseCallback;
  if (!SysWindowInit("Game", 1024, 576)) {
    goto error;
  }

  if (!GpuDeviceInit()) {
    goto error;
  }

  if (!GpuSwapchainInit()) {
    goto error;
  }

  if (!AppInit()) {
    goto error;
  }

  LOG_INFO("Starting main loop");
  SysWindowShow();

  while (!g_exit) {
    SysWindowProcessMessages();
    AppUpdate(0.0f);
    AppRender(1.0f);
    GpuSwapchainPresent();
  }

  g_exitCode = 0;

error:
  LOG_INFO("Exiting application");
  AppDeinit();
  GpuSwapchainDeinit();
  GpuDeviceDeinit();
  SysWindowDeinit();
  return g_exitCode;
}
