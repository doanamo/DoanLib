#pragma once

/*
 * Math macros
 */

#define DN_MAX(a, b) \
  ({ \
    auto _a = (a); \
    auto _b = (b); \
    _a > _b ? _a : _b; \
  })

#define DN_MIN(a, b) \
  ({ \
    auto _a = (a); \
    auto _b = (b); \
    _a < _b ? _a : _b; \
  })

#define DN_IS_POW2(value) \
  ({ \
    u64 _value = (value); \
    _value != 0 && (_value & (_value - 1)) == 0; \
  })
