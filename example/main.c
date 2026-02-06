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

  g_dnSysWindowCloseCallback = &CloseCallback;
  if (!DnSysWindowInit("Example", 1024, 576)) {
    goto error;
  }

  if (!DnGpuDeviceInit()) {
    goto error;
  }

  if (!DnGpuSwapchainInit()) {
    goto error;
  }

  if (!DnAppInit()) {
    goto error;
  }

  LOG_INFO("Starting main loop");
  DnSysWindowShow();

  while (!g_exit) {
    DnSysWindowProcessMessages();
    DnAppUpdate(0.0f);
    DnAppRender(1.0f);
    DnGpuSwapchainPresent();
  }

  g_exitCode = 0;

error:
  LOG_INFO("Exiting application");
  DnAppDeinit();
  DnGpuSwapchainDeinit();
  DnGpuDeviceDeinit();
  DnSysWindowDeinit();
  return g_exitCode;
}
