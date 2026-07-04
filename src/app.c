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

static void DnApp_CloseCallback() {
  g_exit = true;
}

static bool DnApp_Init(DnApp* app, const DnAppConfig* config) {
  DN_LOG_INFO("Initializing application");

  DN_ASSERT(app);
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

  if (!app->init(app, config)) {
    return false;
  }

  return true;
}

static void DnApp_Update(DnApp* app, f32 deltaTime) {
  DN_ASSERT(app);
  DN_UNUSED(deltaTime);

  app->update(app, deltaTime);
}

static void DnApp_Render(DnApp* app, f32 alphaTime) {
  DN_ASSERT(app);
  DN_UNUSED(alphaTime);

  app->render(app, alphaTime);
}

static void DnApp_Deinit(DnApp* app) {
  DN_LOG_INFO("Deinitializing application");

  DN_ASSERT(app);
  app->deinit(app);

  if (g_sysWindow) {
    DnSysWindow_Destroy(g_sysWindow);
    g_sysWindow = nullptr;
  }
}

// == APPLICATION LOOP ====================================================== //

int DnApp_Run(DnApp* app, const DnAppConfig* config) {
  DN_ASSERT(app);
  DN_ASSERT(config);

  if (!DnApp_Init(app, config)) {
    goto error;
  }

  DN_LOG_INFO("Running application");
  DnSysWindow_SetVisibility(g_sysWindow, true);

  while (!g_exit) {
    DnMemTempScope tempScope = DnMemTemp_PushScope();

    DnSysWindow_ProcessMessages(g_sysWindow);
    DnApp_Update(app, 0.0f);
    DnApp_Render(app, 1.0f);

    DnMemTemp_PopScope(&tempScope);
  }

  g_exitCode = 0;

error:
  DnApp_Deinit(app);
  return g_exitCode;
}
