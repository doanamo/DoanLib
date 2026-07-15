#pragma once

#include "shared.h"

// == MATH MACROS =========================================================== //

// Returns the maximum of two values.
#define DN_MAX(a, b) ({ \
    auto _a = (a); \
    auto _b = (b); \
    _a > _b ? _a : _b; \
  })

// Returns the minimum of two values.
#define DN_MIN(a, b) ({ \
    auto _a = (a); \
    auto _b = (b); \
    _a < _b ? _a : _b; \
  })

// Checks whether the given value is a power of 2.
#define DN_IS_POW2(value) ({ \
    u64 _value = (value); \
    _value != 0 && (_value & (_value - 1)) == 0; \
  })

// == MATH VECTORS ========================================================== //

// Struct representing two-dimensional vectors with float components.
typedef struct DnVec2f {
  union {
    struct {
      float x;
      float y;
    };

    float f[2];
  };
} DnVec2f;

// Struct representing three-dimensional vectors with float components.
typedef struct DnVec3f {
  union {
    struct {
      float x;
      float y;
      float z;
    };

    float f[3];
  };
} DnVec3f;

// Struct representing four-dimensional vectors with float components.
typedef struct DnVec4f {
  union {
    struct {
      float x;
      float y;
      float z;
      float w;
    };

    float f[4];
  };
} DnVec4f;

// Struct representing two-dimensional vectors with integer components.
typedef struct DnVec2i {
  union {
    struct {
      i32 x;
      i32 y;
    };

    i32 i[2];
  };
} DnVec2i;

// Struct representing three-dimensional vectors with integer components.
typedef struct DnVec3i {
  union {
    struct {
      i32 x;
      i32 y;
      i32 z;
    };

    i32 i[3];
  };
} DnVec3i;

// Struct representing four-dimensional vectors with integer components.
typedef struct DnVec4i {
  union {
    struct {
      i32 x;
      i32 y;
      i32 z;
      i32 w;
    };

    i32 i[4];
  };
} DnVec4i;

// Struct representing two-dimensional vectors with unsigned integer components.
typedef struct DnVec2u {
  union {
    struct {
      u32 x;
      u32 y;
    };

    u32 u[2];
  };
} DnVec2u;

// Struct representing three-dimensional vectors with unsigned integer components.
typedef struct DnVec3u {
  union {
    struct {
      u32 x;
      u32 y;
      u32 z;
    };

    u32 u[3];
  };
} DnVec3u;

// Struct representing four-dimensional vectors with unsigned integer components.
typedef struct DnVec4u {
  union {
    struct {
      u32 x;
      u32 y;
      u32 z;
      u32 w;
    };

    u32 u[4];
  };
} DnVec4u;

// == MATHS COLORS ========================================================== //

// Struct representing colors with four byte components.
typedef struct DnColor {
  union {

    u8 c[4];

    struct {
      u8 r;
      u8 g;
      u8 b;
      u8 a;
    };

  };
} DnColor;

// Predefined color constants.
constexpr DnColor DnColor_White = { .r = 255, .g = 255, .b = 255, .a = 255 };
constexpr DnColor DnColor_Black = { .r = 0, .g = 0, .b = 0, .a = 255 };
constexpr DnColor DnColor_Red = { .r = 255, .g = 0, .b = 0, .a = 255 };
constexpr DnColor DnColor_Green = { .r = 0, .g = 255, .b = 0, .a = 255 };
constexpr DnColor DnColor_Blue = { .r = 0, .g = 0, .b = 255, .a = 255 };
constexpr DnColor DnColor_Win98 = { .r = 0, .g = 128, .b = 128, .a = 255 };
