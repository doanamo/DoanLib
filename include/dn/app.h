#pragma once

#include "dn/structs.h"

// == APPLICATION =========================================================== //

// Application configurations struct.
typedef struct {
  DnStrView windowTitle;
  u32 windowWidth;
  u32 windowHeight;
} DnAppConfig;

// Application interface for caller to implement.
typedef struct DnApp {
  bool (*init)(struct DnApp* app, const DnAppConfig* config);
  void (*update)(struct DnApp* app, float deltaTime);
  void (*render)(struct DnApp* app, float alphaTime);
  void (*deinit)(struct DnApp* app);
  void* userdata;
} DnApp;

// Runs the application with the given configuration. Does not return until the
// application exits. Returns the exit code.
int DnApp_Run(DnApp* app, const DnAppConfig* config);
