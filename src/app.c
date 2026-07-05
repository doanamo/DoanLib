#include "dn/app.h"
#include "dn/memory.h"
#include "dn/shared.h"
#include "dn/structs.h"
#include "dn/system.h"
#include "dn/main.h"

// == APPLICATION METHODS =================================================== //

static void DnApp_CloseCallback(bool* closing, void* userdata) {
  DN_UNUSED(closing);
  DnApp* app = (DnApp*)userdata;
  DN_ASSERT(app);
}

static bool DnApp_Init(DnApp* app, const DnAppConfig* config) {
  DN_LOG_INFO("Initializing application");

  DN_ASSERT(app);
  DN_ASSERT(config);

  DN_ASSERT(!app->window);
  app->window = DnSysWindow_Create();
  if (!app->window) {
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

  DnSysWindow_SetTitle(app->window, windowTitle);
  DnSysWindow_SetSize(app->window, windowWidth, windowHeight);
  DnSysWindow_SetCloseCallback(app->window, &DnApp_CloseCallback, app);

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

  if (app->window) {
    DnSysWindow_Destroy(app->window);
    app->window = nullptr;
  }
}

// == APPLICATION LOOP ====================================================== //

int DnApp_Run(DnApp* app, const DnAppConfig* config) {
  DN_ASSERT(app);
  DN_ASSERT(config);

  DnExitCode exitCode = DnExitCode_UnknownFailure;
  if (!DnApp_Init(app, config)) {
    exitCode = DnExitCode_AppInitFailure;
    goto error;
  }

  DN_LOG_INFO("Running application");
  DnSysWindow_SetVisibility(app->window, true);

  while (!DnSysWindow_IsClosing(app->window)) {
    DnMemTempScope tempScope = DnMemTemp_PushScope();

    DnSysWindow_ProcessMessages(app->window);
    DnApp_Update(app, 0.0f);
    DnApp_Render(app, 1.0f);

    DnMemTemp_PopScope(&tempScope);
  }

  exitCode = DnExitCode_Success;

error:
  DnApp_Deinit(app);
  return exitCode;
}
