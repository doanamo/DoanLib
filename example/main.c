#include "dn/main.h"
#include "dn/app.h"

bool Example_Init(DnApp* app, const DnAppConfig* config) {
  DN_UNUSED(app);
  DN_UNUSED(config);
  return true;
}

void Example_Update(DnApp* app, float deltaTime) {
  DN_UNUSED(app);
  DN_UNUSED(deltaTime);
}

void Example_Render(DnApp* app, float alphaTime) {
  DN_UNUSED(app);
  DN_UNUSED(alphaTime);
}

void Example_Deinit(DnApp* app) {
  DN_UNUSED(app);
}

DN_DEFINE_MAIN_ENTRY() {
  DnApp example = {
    .init = Example_Init,
    .update = Example_Update,
    .render = Example_Render,
    .deinit = Example_Deinit,
    .userdata = nullptr,
  };

  DnAppConfig config = {
    .windowTitle = DN_STR_VIEW_LITERAL("DoanLib Example"),
    .windowWidth = 1024,
    .windowHeight = 576,
  };

  return DnApp_Run(&example, &config);
}
