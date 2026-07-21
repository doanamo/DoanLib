#include "dn/main.h"
#include "dn/app.h"
#include "dn/system.h"
#include "dn/raster.h"

// == EXAMPLE APPLICATION =================================================== //

typedef struct ExampleApp {
  DnApp app;
  DnRasterTexture texture;
} ExampleApp;

bool ExampleApp_Init(DnApp* app, const DnAppConfig* config) {
  DN_UNUSED(app);
  DN_UNUSED(config);

  ExampleApp* example = (ExampleApp*)app;
  DN_ASSERT(example);

  if (!DnRasterTexture_Init(&example->texture, 320, 180)) {
    return false;
  }

  DnRasterTexture* texture = &example->texture;
  DnRasterTexture_Clear(texture, DnColor_Win98);

  DnRaster_Line(texture, (DnVec2i){ .x = 7, .y = 3 }, (DnVec2i){ .x = 12, .y = 37 }, DnColor_Red);
  DnRaster_Line(texture, (DnVec2i){ .x = 62, .y = 53 }, (DnVec2i){ .x = 7, .y = 3 }, DnColor_Green);
  DnRaster_Line(texture, (DnVec2i){ .x = 12, .y = 37 }, (DnVec2i){ .x = 62, .y = 53 }, DnColor_Blue);

  DnRaster_Point(texture, (DnVec2i){ .x = 7, .y = 3 }, DnColor_White);
  DnRaster_Point(texture, (DnVec2i){ .x = 12, .y = 37 }, DnColor_White);
  DnRaster_Point(texture, (DnVec2i){ .x = 62, .y = 53 }, DnColor_White);

  return true;
}

void ExampleApp_Update(DnApp* app, float deltaTime) {
  DN_UNUSED(app);
  DN_UNUSED(deltaTime);

  ExampleApp* example = (ExampleApp*)app;
  DN_ASSERT(example);
  DN_UNUSED(example);
}

void ExampleApp_Render(DnApp* app, float alphaTime) {
  DN_UNUSED(app);
  DN_UNUSED(alphaTime);

  ExampleApp* example = (ExampleApp*)app;
  DN_ASSERT(example);

  DnRasterTexture* texture = &example->texture;
  DnSysWindow_Present(app->window, (u32*)texture->data, texture->width, texture->height);
}

void ExampleApp_Deinit(DnApp* app) {
  DN_UNUSED(app);

  ExampleApp* example = (ExampleApp*)app;
  DN_ASSERT(example);

  DnRasterTexture_Deinit(&example->texture);
}

void ExampleApp_OnResize(DnApp* app, i32 width, i32 height) {
  DN_UNUSED(app);
  DN_UNUSED(width);
  DN_UNUSED(height);
}

void ExampleApp_OnClose(DnApp* app, bool* close) {
  DN_UNUSED(app);
  DN_UNUSED(close);
}

// == MAIN ================================================================== //

DN_DEFINE_MAIN_ENTRY() {
  ExampleApp example = {
    .app = {
      .init = ExampleApp_Init,
      .update = ExampleApp_Update,
      .render = ExampleApp_Render,
      .deinit = ExampleApp_Deinit,
      .onResize = ExampleApp_OnResize,
      .onClose = ExampleApp_OnClose,
    },
  };

  DnAppConfig config = {
    .windowTitle = DN_STR_VIEW_LITERAL("DoanLib Example"),
    .windowWidth = 1024,
    .windowHeight = 576,
  };

  return DnApp_Run(&example.app, &config);
}
