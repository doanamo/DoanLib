#pragma once

#include "shared.h"
#include "math.h"

// == RASTERIZATION TEXTURE ================================================= //

// Struct representing 2D texture onto which rasterization can be performed.
typedef struct DnRasterTexture {
  u32 width;
  u32 height;
  DnColor* data;
} DnRasterTexture;

// Initializes raster texture.
bool DnRasterTexture_Init(DnRasterTexture* texture, u32 width, u32 height);

// Deinitializes raster texture.
void DnRasterTexture_Deinit(DnRasterTexture* texture);

// Clears raster texture data.
void DnRasterTexture_Clear(DnRasterTexture* texture, DnColor color);

// Sets pixel on raster texture.
void DnRasterTexture_Set(DnRasterTexture* texture, u32 x, u32 y, DnColor color);

// == RASTERIZATION SHAPES ================================================== //

void DnRaster_Line(DnVec2f start, DnVec2f end);
