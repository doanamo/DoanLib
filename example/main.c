#include "dn/main.h"
#include "dn/app.h"
#include "dn/memory.h"
#include "dn/system.h"

// == EXAMPLE APPLICATION =================================================== //

typedef struct ExampleApp {
  DnApp app;
  u32 width;
  u32 height;
  DnColor* pixels;
} ExampleApp;

bool ExampleApp_Init(DnApp* app, const DnAppConfig* config) {
  DN_UNUSED(app);
  DN_UNUSED(config);

  ExampleApp* example = (ExampleApp*)app;
  DN_ASSERT(example);

  DN_ASSERT(!example->pixels);
  example->pixels = DN_MEM_ALLOC_TYPES(DnMemLarge_GetAllocator(),
    DnColor, example->width * example->height);

  for (u32 y = 0; y < example->height; ++y) {
    for (u32 x = 0; x < example->width; ++x) {
      DnColor pixel = {
        .r = 0,
        .g = 132,
        .b = 137,
        .a = 255
      };

      example->pixels[y * example->width + x] = pixel;
    }
  }

  return true;
}

void ExampleApp_Update(DnApp* app, float deltaTime) {
  DN_UNUSED(app);
  DN_UNUSED(deltaTime);

  ExampleApp* example = (ExampleApp*)app;
  DN_ASSERT(example);
}

void ExampleApp_Render(DnApp* app, float alphaTime) {
  DN_UNUSED(app);
  DN_UNUSED(alphaTime);

  ExampleApp* example = (ExampleApp*)app;
  DN_ASSERT(example);

  DnSysWindow_Present(app->window, (u32*)example->pixels, example->width, example->height);
}

void ExampleApp_Deinit(DnApp* app) {
  DN_UNUSED(app);

  ExampleApp* example = (ExampleApp*)app;
  DN_ASSERT(example);

  DN_MEM_FREE(DnMemLarge_GetAllocator(), example->pixels);
}

// == MAIN ================================================================== //

DN_DEFINE_MAIN_ENTRY() {
  ExampleApp example = {
    .app = {
      .init = ExampleApp_Init,
      .update = ExampleApp_Update,
      .render = ExampleApp_Render,
      .deinit = ExampleApp_Deinit,
    },
    .width = 320,
    .height = 180,
    .pixels = nullptr,
  };

  DnAppConfig config = {
    .windowTitle = DN_STR_VIEW_LITERAL("DoanLib Example"),
    .windowWidth = 1024,
    .windowHeight = 576,
  };

  return DnApp_Run(&example.app, &config);
}
