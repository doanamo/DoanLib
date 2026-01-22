#ifndef SHARED_H
#define SHARED_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#define UNUSED(x) (void)(x)

#define KiB(n) ((size_t)(n) << 10)
#define MiB(n) ((size_t)(n) << 20)
#define GiB(n) ((size_t)(n) << 30)

#define MAX(a, b) \
  ({ \
    auto _a = (a); \
    auto _b = (b); \
    _a > _b ? _a : _b; \
  })

#define MIN(a, b) \
  ({ \
    auto _a = (a); \
    auto _b = (b); \
    _a < _b ? _a : _b; \
  })

#define LOG_INFO(format, ...) \
  fprintf(stdout, format "\n" __VA_OPT__(,) __VA_ARGS__); \
  fflush(stdout)

#define LOG_ERROR(format, ...) \
  fprintf(stderr, format "\n" __VA_OPT__(,) __VA_ARGS__); \
  fflush(stdout)

#endif
