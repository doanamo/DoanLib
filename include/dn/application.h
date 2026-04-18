#pragma once

#include "dn/memory.h"
#include "dn/system.h"

/*
 * Application
 */

typedef struct DnAppConfig {
  DnMemConfig memory;
  DnSysConfig system;
} DnAppConfig;

int DnApp_Run(const DnAppConfig* config);
