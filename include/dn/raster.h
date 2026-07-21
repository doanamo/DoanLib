#pragma once

#include "shared.h"
#include "math.h"

// == RASTERIZATION TEXTURE ================================================= //

// Struct representing 2D texture onto which rasterization can be performed.
typedef struct DnRasterTexture {
  i32 width;
  i32 height;
  DnColor* data;
} DnRasterTexture;

// Initializes raster texture.
bool DnRasterTexture_Init(DnRasterTexture* texture, i32 width, i32 height);

// Deinitializes raster texture.
void DnRasterTexture_Deinit(DnRasterTexture* texture);

// Clears raster texture data.
void DnRasterTexture_Clear(DnRasterTexture* texture, DnColor color);

// Sets pixel on raster texture.
void DnRasterTexture_Set(DnRasterTexture* texture, i32 x, i32 y, DnColor color);

// == RASTERIZATION SHAPES ================================================== //

void DnRaster_Point(DnRasterTexture* texture, DnVec2i position, DnColor color);
void DnRaster_Line(DnRasterTexture* texture, DnVec2i start, DnVec2i end, DnColor color);
