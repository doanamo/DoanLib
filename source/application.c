#include "shared.h"
#include "application.h"

bool application_init() {
  LOG_INFO("Initializing application");
  return true;
}

void application_update(float delta_time) {
  UNUSED(delta_time);
}

void application_render(float alpha_time) {
  UNUSED(alpha_time);
}

void application_deinit() {
  LOG_INFO("Deinitializing application");
}
