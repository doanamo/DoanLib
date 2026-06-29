#pragma once

#include "memory.h"
#include "system.h"

// == APPLICATION =========================================================== //

// Application configurations struct.
typedef struct DnAppConfig {
  DnMemConfig memory;
  DnSysConfig system;
} DnAppConfig;

// Runs the application with the given configuration. Does not return until the
// application exits. Returns the exit code.
int DnApp_Run(const DnAppConfig* config);
