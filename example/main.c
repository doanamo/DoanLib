#include "dn/main.h"
#include "dn/app.h"
#include "dn/memory.h"
#include "dn/system.h"

// == EXAMPLE APPLICATION =================================================== //

typedef struct Example {
  DnApp app;
  u32 width;
  u32 height;
  u32* pixels;
} Example;

bool Example_Init(DnApp* app, const DnAppConfig* config) {
  DN_UNUSED(app);
  DN_UNUSED(config);

  Example* example = (Example*)app;
  DN_ASSERT(example);

  DN_ASSERT(!example->pixels);
  example->pixels = DN_MEM_ALLOC(DnMemLarge_GetAllocator(),
    example->width * example->height * sizeof(u32),
    DnMem_DefaultAlignment);

  for (u32 y = 0; y < example->height; ++y) {
    for (u32 x = 0; x < example->width; ++x) {
      u8 r = 0, g = 132, b = 137;
      example->pixels[y * example->width + x] =
        ((u32)r << 16) | ((u32)g << 8) | (u32)b;
    }
  }

  return true;
}

void Example_Update(DnApp* app, float deltaTime) {
  DN_UNUSED(app);
  DN_UNUSED(deltaTime);

  Example* example = (Example*)app;
  DN_ASSERT(example);
}

void Example_Render(DnApp* app, float alphaTime) {
  DN_UNUSED(app);
  DN_UNUSED(alphaTime);

  Example* example = (Example*)app;
  DN_ASSERT(example);

  DnSysWindow_Present(app->window, example->pixels, example->width, example->height);
}

void Example_Deinit(DnApp* app) {
  DN_UNUSED(app);

  Example* example = (Example*)app;
  DN_ASSERT(example);

  DN_MEM_FREE(DnMemLarge_GetAllocator(), example->pixels);
}

// == MAIN ================================================================== //

DN_DEFINE_MAIN_ENTRY() {
  Example example = {
    .app = {
      .init = Example_Init,
      .update = Example_Update,
      .render = Example_Render,
      .deinit = Example_Deinit,
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
