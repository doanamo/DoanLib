#pragma once

#include "shared.h"

// == MATH MACROS =========================================================== //

// Returns the maximum of two values.
#define DN_MAX(a, b) \
  ({ \
    auto _a = (a); \
    auto _b = (b); \
    _a > _b ? _a : _b; \
  })

// Returns the minimum of two values.
#define DN_MIN(a, b) \
  ({ \
    auto _a = (a); \
    auto _b = (b); \
    _a < _b ? _a : _b; \
  })

// Checks whether the given value is a power of 2.
#define DN_IS_POW2(value) \
  ({ \
    u64 _value = (value); \
    _value != 0 && (_value & (_value - 1)) == 0; \
  })

// == MATH VECTORS ========================================================== //

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
