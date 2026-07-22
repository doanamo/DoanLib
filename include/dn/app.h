#pragma once

#include "dn/structs.h"

// == APPLICATION =========================================================== //

typedef struct DnSysWindow DnSysWindow;

// Application configurations struct.
typedef struct {
  DnStrView windowTitle;
  i32 windowWidth;
  i32 windowHeight;
} DnAppConfig;

// Application interface for caller to implement.
typedef struct DnApp {
  bool (*init)(struct DnApp* app);
  void (*update)(struct DnApp* app, f32 deltaTime);
  void (*render)(struct DnApp* app, f32 alphaTime);
  void (*deinit)(struct DnApp* app);

  void (*onResize)(struct DnApp* app, i32 width, i32 height);
  void (*onClose)(struct DnApp* app, bool* close);

  DnSysWindow* window;
} DnApp;

// Runs the application with the given configuration. Does not return until the
// application exits. Returns the exit code.
int DnApp_Run(DnApp* app, const DnAppConfig* config);
