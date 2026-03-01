#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

#define DN_UNUSED(x) (void)(x)
#define DN_ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

#define DN_BREAK() __builtin_debugtrap()
#define DN_ABORT() __builtin_trap()

#define DN_ASSERT_ALWAYS(expression) \
  if (!(expression)) { \
    DN_LOG_ERROR("Assertion failed: %s", #expression); \
    DN_ABORT(); \
  }

#ifndef DN_CONFIG_RELEASE
  #define DN_ASSERT(expression) DN_ASSERT_ALWAYS(expression)
  #define DN_ASSERT_EVALUATE(expression) DN_ASSERT_ALWAYS(expression)

  #define DN_LOG_INFO(format, ...) \
    fprintf(stdout, format "\n" __VA_OPT__(,) __VA_ARGS__); \
    fflush(stdout)

  #define DN_LOG_ERROR(format, ...) \
    fprintf(stderr, format "\n" __VA_OPT__(,) __VA_ARGS__); \
    fflush(stdout); \
    fflush(stderr)
#else
  #define DN_ASSERT(expression)
  #define DN_ASSERT_EVALUATE(expression) (void)(expression)
  #define DN_LOG_INFO(format, ...)
  #define DN_LOG_ERROR(format, ...)
#endif

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

#define DN_DEFAULT_ALIGNMENT alignof(max_align_t)
#define DN_ALIGN_UP(size, alignment) \
  ({ \
    u64 _size = (size); \
    u64 _alignment = (alignment); \
    DN_ASSERT(DN_IS_POW2(_alignment)); \
    (_size + (_alignment - 1)) & ~(_alignment - 1); \
  })

#define DN_KiB(size) \
  ({ \
    u64 _size = (size); \
    _size << 10; \
  })

#define DN_MiB(size) \
  ({ \
    u64 _size = (size); \
    _size << 20; \
  })

#define DN_GiB(size) \
  ({ \
    u64 _size = (size); \
    _size << 30; \
  })

#include "memory.h"
