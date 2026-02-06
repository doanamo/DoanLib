#include "../include/dn/shared.h"
#include "../include/dn/application.h"

bool DnAppInit() {
  LOG_INFO("Initializing application");
  return true;
}

void DnAppUpdate(float deltaTime) {
  UNUSED(deltaTime);
}

void DnAppRender(float alphaTime) {
  UNUSED(alphaTime);
}

void DnAppDeinit() {
  LOG_INFO("Deinitializing application");
}
