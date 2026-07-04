#include "dn/app.h"
#include "dn/memory.h"
#include "dn/shared.h"
#include "dn/structs.h"
#include "dn/system.h"

// == APPLICATION GLOBALS =================================================== //

static DnSysWindow* g_sysWindow = nullptr;

static bool g_exit = false;
static int g_exitCode = 1;

// == APPLICATION METHODS =================================================== //

void DnApp_CloseCallback() {
  g_exit = true;
}

bool DnApp_Init(const DnAppConfig* config) {
  DN_LOG_INFO("Initializing application");
  DN_ASSERT(config);

  g_sysWindow = DnSysWindow_Create();
  if (!g_sysWindow) {
    return false;
  }

  DnStrView windowTitle = config->windowTitle;
  if (DnStrView_IsEmpty(windowTitle)) {
    windowTitle = DN_STR_VIEW_LITERAL("DoanLib Window");
  }

  u32 windowWidth = config->windowWidth;
  u32 windowHeight = config->windowHeight;
  if (!windowWidth || !windowHeight) {
    windowWidth = 1024;
    windowHeight = 576;
  }

  DnSysWindow_SetTitle(g_sysWindow, windowTitle);
  DnSysWindow_SetSize(g_sysWindow, windowWidth, windowHeight);
  DnSysWindow_SetCloseCallback(g_sysWindow, &DnApp_CloseCallback);

  return true;
}

void DnApp_Update(f32 deltaTime) {
  DN_UNUSED(deltaTime);
}

void DnApp_Render(f32 alphaTime) {
  DN_UNUSED(alphaTime);
}

void DnApp_Deinit() {
  DN_LOG_INFO("Deinitializing application");

  if (g_sysWindow) {
    DnSysWindow_Destroy(g_sysWindow);
    g_sysWindow = nullptr;
  }
}

// == APPLICATION LOOP ====================================================== //

int DnApp_Run(const DnAppConfig* config) {
  DN_ASSERT(config);
  if (!DnApp_Init(config)) {
    goto error;
  }

  DN_LOG_INFO("Running application");
  DnSysWindow_SetVisibility(g_sysWindow, true);

  while (!g_exit) {
    DnMemTempScope tempScope = DnMemTemp_PushScope();

    DnSysWindow_ProcessMessages(g_sysWindow);
    DnApp_Update(0.0f);
    DnApp_Render(1.0f);

    DnMemTemp_PopScope(&tempScope);
  }

  g_exitCode = 0;

error:
  DnApp_Deinit();
  return g_exitCode;
}
