#pragma once

/*
 * Standard includes
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/*
 * System includes
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/*
 * Predefined types
 */

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

/*
 * Compiler macros
 */

#define DN_UNUSED(x) (void)(x)
#define DN_ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

#define DN_BREAK() __builtin_debugtrap()
#define DN_ABORT() __builtin_trap()

/*
 * Logging macros
 */

void DnLog_Info(const char* format, ...);
void DnLog_Error(const char* format, ...);

#ifndef DN_CONFIG_RELEASE
  #define DN_LOG_INFO(format, ...) DnLog_Info(format "\n" __VA_OPT__(,) __VA_ARGS__)
  #define DN_LOG_ERROR(format, ...) DnLog_Error(format "\n" __VA_OPT__(,) __VA_ARGS__)
#else
  #define DN_LOG_INFO(format, ...)
  #define DN_LOG_ERROR(format, ...)
#endif

/*
 * Assertion macros
 */

#define DN_ASSERT_ALWAYS(expression) \
  if (!(expression)) { \
    DN_LOG_ERROR("Assertion failed: %s", #expression); \
    DN_ABORT(); \
  }

#ifndef DN_CONFIG_RELEASE
  #define DN_ASSERT(expression) DN_ASSERT_ALWAYS(expression)
  #define DN_ASSERT_EVALUATE(expression) DN_ASSERT_ALWAYS(expression)
#else
  #define DN_ASSERT(expression)
  #define DN_ASSERT_EVALUATE(expression) (void)(expression)
#endif
