#pragma once

#include "dn/structs.h"

// == APPLICATION =========================================================== //

// Application configurations struct.
typedef struct DnAppConfig {
  DnStrView windowTitle;
  u32 windowWidth;
  u32 windowHeight;
} DnAppConfig;

// Runs the application with the given configuration. Does not return until the
// application exits. Returns the exit code.
int DnApp_Run(const DnAppConfig* config);
