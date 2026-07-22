#include "dn/raster.h"
#include "dn/memory.h"
#include <math.h>

// == RASTERIZATION TEXTURE ================================================= //

bool DnRasterTexture_Init(DnRasterTexture* texture, i32 width, i32 height) {
  DN_ASSERT(texture);
  DN_ASSERT(width > 0);
  DN_ASSERT(height > 0);

  texture->width = width;
  texture->height = height;

  DN_ASSERT(!texture->data);
  texture->data = DN_MEM_ALLOC_TYPES(DnMemLarge_GetAllocator(),
    DnColor, (u32)texture->width * (u32)texture->height);

  return true;
}

void DnRasterTexture_Deinit(DnRasterTexture* texture) {
  DN_ASSERT(texture);

  if (texture->data) {
    DN_MEM_FREE(DnMemLarge_GetAllocator(), texture->data);
  }
}

void DnRasterTexture_Clear(DnRasterTexture* texture, DnColor color) {
  DN_ASSERT(texture);
  DN_ASSERT(texture->data);

  // #todo: Implement fast path using memset.

  for (i32 y = 0; y < texture->height; ++y) {
    for (i32 x = 0; x < texture->width; ++x) {
      texture->data[y * texture->width + x] = color;
    }
  }
}

void DnRasterTexture_Set(DnRasterTexture* texture, i32 x, i32 y, DnColor color) {
  DN_ASSERT(texture);
  DN_ASSERT(texture->data);

  if (x >= 0 && y >= 0 && x < (i32)texture->width && y < (i32)texture->height) {
    texture->data[y * texture->width + x] = color;
  }
}

void DnRaster_Point(DnRasterTexture* texture, DnVec2i position, DnColor color) {
  DnRasterTexture_Set(texture, position.x, position.y, color);
}

void DnRaster_Line(DnRasterTexture* texture, DnVec2i start, DnVec2i end, DnColor color) {
  bool steep = abs(start.x - end.x) < abs(start.y - end.y);

  if (steep) {
    DN_SWAP(start.x, start.y);
    DN_SWAP(end.x, end.y);
  }

  if (start.x > end.x) {
    DN_SWAP(start.x, end.x);
    DN_SWAP(start.y, end.y);
  }

  for (i32 x = start.x; x <= end.x; ++x) {
    f32 t = (f32)(x - start.x) / (f32)(end.x - start.x);
    i32 y = (i32)((f32)start.y + (f32)(end.y - start.y) * t + 0.5f);

    if (steep) {
      DnRasterTexture_Set(texture, y, x, color);
    }
    else {
      DnRasterTexture_Set(texture, x, y, color);
    }
  }
}
