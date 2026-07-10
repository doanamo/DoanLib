#include "dn/raster.h"
#include "dn/memory.h"

// == RASTERIZATION TEXTURE ================================================= //

bool DnRasterTexture_Init(DnRasterTexture* texture, u32 width, u32 height) {
  DN_ASSERT(texture);
  texture->width = width;
  texture->height = height;

  DN_ASSERT(!texture->data);
  texture->data = DN_MEM_ALLOC_TYPES(DnMemLarge_GetAllocator(),
    DnColor, texture->width * texture->height);

  return true;
}

void DnRasterTexture_Deinit(DnRasterTexture* texture) {
  DN_ASSERT(texture);
  DN_MEM_FREE(DnMemLarge_GetAllocator(), texture->data);
}

void DnRasterTexture_Clear(DnRasterTexture* texture, DnColor color) {
  DN_ASSERT(texture);
  DN_ASSERT(texture->data);

  // #todo: Implement fast path using memset.

  for (u32 y = 0; y < texture->height; ++y) {
    for (u32 x = 0; x < texture->width; ++x) {
      texture->data[y * texture->width + x] = color;
    }
  }
}

void DnRasterTexture_Set(DnRasterTexture* texture, u32 x, u32 y, DnColor color) {
  DN_ASSERT(texture);
  DN_ASSERT(texture->data);
  DN_ASSERT(x < texture->width);
  DN_ASSERT(y < texture->height);

  if (x < texture->width && y < texture->height) {
    texture->data[y * texture->width + x] = color;
  }
}

void DnRaster_Point(DnRasterTexture* texture, DnVec2f position, DnColor color) {
  DnRasterTexture_Set(texture, (u32)position.x, (u32)position.y, color);
}

void DnRaster_Line(DnRasterTexture* texture, DnVec2f start, DnVec2f end, DnColor color) {
  for (float t = 0; t <= 1; t += 0.02f) {
    float x = start.x + t * (end.x - start.x) + 0.5f;
    float y = start.y + t * (end.y - start.y) + 0.5f;
    DnRasterTexture_Set(texture, (u32)x, (u32)y, color);
  }
}
