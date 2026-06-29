#pragma once

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
