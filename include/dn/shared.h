#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define COBJMACROS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dxgi1_6.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <dxgidebug.h>

#ifndef DN_CONFIG_RELEASE
  #define DN_LOG_INFO(format, ...) \
    fprintf(stdout, format "\n" __VA_OPT__(,) __VA_ARGS__);

  #define DN_LOG_ERROR(format, ...) \
    fprintf(stderr, format "\n" __VA_OPT__(,) __VA_ARGS__); \
    fflush(stdout); \
    fflush(stderr)
#else
  #define DN_LOG_INFO(format, ...)
  #define DN_LOG_ERROR(format, ...)
#endif

#define DN_UNUSED(x) (void)(x)
#define DN_ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

#define DN_KiB(n) ((size_t)(n) << 10)
#define DN_MiB(n) ((size_t)(n) << 20)
#define DN_GiB(n) ((size_t)(n) << 30)

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
