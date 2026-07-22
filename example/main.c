#include "dn/main.h"
#include "dn/app.h"
#include "dn/system.h"
#include "dn/raster.h"

// == APPLICATION =========================================================== //

typedef struct DnAppExample {
  DnApp app;
  DnRasterTexture texture;
} DnAppExample;

bool DnAppExample_Init(DnApp* app) {
  DnAppExample* exampleApp = (DnAppExample*)app;
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

void DnAppExample_Update(DnApp* app, f32 deltaTime) {
  DN_UNUSED(deltaTime);

  DnAppExample* exampleApp = (DnAppExample*)app;
  DN_ASSERT(exampleApp);
  DN_UNUSED(exampleApp);
}

void DnAppExample_Render(DnApp* app, f32 alphaTime) {
  DN_UNUSED(alphaTime);

  DnAppExample* exampleApp = (DnAppExample*)app;
  DN_ASSERT(exampleApp);

  DnRasterTexture* texture = &exampleApp->texture;
  DnSysWindow_Present(app->window, (u32*)texture->data, texture->width, texture->height);
}

void DnAppExample_Deinit(DnApp* app) {
  DnAppExample* exampleApp = (DnAppExample*)app;
  DN_ASSERT(exampleApp);

  DnRasterTexture_Deinit(&exampleApp->texture);
}

void DnAppExample_OnResize(DnApp* app, i32 width, i32 height) {
  DN_UNUSED(app);
  DN_UNUSED(width);
  DN_UNUSED(height);
}

void DnAppExample_OnClose(DnApp* app, bool* close) {
  DN_UNUSED(app);
  DN_UNUSED(close);
}

// == MAIN ================================================================== //

DN_DEFINE_MAIN_ENTRY() {
  DnAppExample exampleApp = {
    .app = {
      .init = DnAppExample_Init,
      .update = DnAppExample_Update,
      .render = DnAppExample_Render,
      .deinit = DnAppExample_Deinit,
      .onResize = DnAppExample_OnResize,
      .onClose = DnAppExample_OnClose,
    },
  };

  DnAppConfig config = {
    .windowTitle = DN_STR_VIEW_LITERAL("DoanExample"),
    .windowWidth = 1024,
    .windowHeight = 576,
  };

  return DnApp_Run(&exampleApp.app, &config);
}
