#include "dn/main.h"
#include "dn/app.h"
#include "dn/system.h"
#include "dn/raster.h"

// == APPLICATION =========================================================== //

typedef struct DnExampleApp {
  DnApp app;
  DnRasterTexture texture;
} DnExampleApp;

bool ExampleApp_Init(DnApp* app) {
  DnExampleApp* exampleApp = (DnExampleApp*)app;
  DN_ASSERT(exampleApp);

  if (!DnRasterTexture_Init(&exampleApp->texture, 320, 180)) {
    return false;
  }

  DnRasterTexture* texture = &exampleApp->texture;
  DnRasterTexture_Clear(texture, DnColor_Win98);

  DnRaster_Line(texture, (DnVec2i){ .x = 7, .y = 3 }, (DnVec2i){ .x = 12, .y = 37 }, DnColor_Red);
  DnRaster_Line(texture, (DnVec2i){ .x = 62, .y = 53 }, (DnVec2i){ .x = 7, .y = 3 }, DnColor_Green);
  DnRaster_Line(texture, (DnVec2i){ .x = 12, .y = 37 }, (DnVec2i){ .x = 62, .y = 53 }, DnColor_Blue);

  DnRaster_Point(texture, (DnVec2i){ .x = 7, .y = 3 }, DnColor_White);
  DnRaster_Point(texture, (DnVec2i){ .x = 12, .y = 37 }, DnColor_White);
  DnRaster_Point(texture, (DnVec2i){ .x = 62, .y = 53 }, DnColor_White);

  return true;
}

void ExampleApp_Update(DnApp* app, f32 deltaTime) {
  DN_UNUSED(deltaTime);

  DnExampleApp* exampleApp = (DnExampleApp*)app;
  DN_ASSERT(exampleApp);
  DN_UNUSED(exampleApp);
}

void ExampleApp_Render(DnApp* app, f32 alphaTime) {
  DN_UNUSED(alphaTime);

  DnExampleApp* exampleApp = (DnExampleApp*)app;
  DN_ASSERT(exampleApp);

  DnRasterTexture* texture = &exampleApp->texture;
  DnSysWindow_Present(app->window, (u32*)texture->data, texture->width, texture->height);
}

void ExampleApp_Deinit(DnApp* app) {
  DnExampleApp* exampleApp = (DnExampleApp*)app;
  DN_ASSERT(exampleApp);

  DnRasterTexture_Deinit(&exampleApp->texture);
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
  DnExampleApp exampleApp = {
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
    .windowTitle = DN_STR_VIEW_LITERAL("DoanExample"),
    .windowWidth = 1024,
    .windowHeight = 576,
  };

  return DnApp_Run(&exampleApp.app, &config);
}
