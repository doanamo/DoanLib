#include "shared.h"
#include "application.h"

bool AppInit() {
  LOG_INFO("Initializing application");
  return true;
}

void AppUpdate(float deltaTime) {
  UNUSED(deltaTime);
}

void AppRender(float alphaTime) {
  UNUSED(alphaTime);
}

void AppDeinit() {
  LOG_INFO("Deinitializing application");
}
