#pragma once

#include "memory.h"
#include "system.h"

/*
 * Application
 */

typedef struct DnAppConfig {
  DnMemConfig memory;
  DnSysConfig system;
} DnAppConfig;

int DnApp_Run(const DnAppConfig* config);
