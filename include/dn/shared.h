#pragma once

// == STANDARD INCLUDES ===================================================== //

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <memory.h>
#include <string.h>

// == PREDEFINED TYPES ====================================================== //

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

// == GLOBAL DEFINES ======================================================== //

// Defines whether logging is enabled.
#define DN_LOG_ENABLED !DN_CONFIG_RELEASE

// Defines whether assertions are enabled.
#define DN_ASSERT_ENABLED !DN_CONFIG_RELEASE

// == COMPILER MACROS ======================================================= //

// Marks variable as intentionally unused to suppress compiler warnings.
#define DN_UNUSED(x) (void)(x)

// Marks an expression as likely to be true, for use in branch prediction.
#define DN_LIKELY(expression) __builtin_expect(!!(expression), 1)

// Marks an expression as unlikely to be true, for use in branch prediction.
#define DN_UNLIKELY(expression) __builtin_expect(!!(expression), 0)

// Triggers a breakpoint in the debugger.
#define DN_BREAK() __builtin_debugtrap()

// Aborts the process.
#define DN_ABORT() __builtin_trap()

// == COMMON MACROS ========================================================= //

// Returns the length of a statically allocated array.
#define DN_ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

// Swaps the values of two variables of the same type.
#define DN_SWAP(a, b) ({ \
    typeof(a) _temp = a; \
    a = b; \
    b = _temp; \
  })

// == LOGGING MACROS ======================================================== //

#if DN_LOG_ENABLED
  void DnLog_Info(const char* format, ...);
  void DnLog_Error(const char* format, ...);

  // Logs an info message using printf-style formatting.
  #define DN_LOG_INFO(format, ...) DnLog_Info(format "\n" __VA_OPT__(,) __VA_ARGS__)

  // Logs an error message using printf-style formatting.
  #define DN_LOG_ERROR(format, ...) DnLog_Error(format "\n" __VA_OPT__(,) __VA_ARGS__)
#else
  #define DN_LOG_INFO(format, ...)
  #define DN_LOG_ERROR(format, ...)
#endif // DN_LOG_ENABLED

// == ASSERTION MACROS ====================================================== //

// Implementation of the assertion macro. Should not be used directly.
#define DN_ASSERT_IMPLEMENTATION(expression, expressionString) \
  do { \
    if (DN_UNLIKELY(!(expression))) { \
      DN_LOG_ERROR("Assertion failed: %s", expressionString); \
      DN_ABORT(); \
    } \
  } while (0)

// Assertion macro that is always executed, even when assertions are disabled.
#define DN_ASSERT_ALWAYS(expression) \
  DN_ASSERT_IMPLEMENTATION(expression, #expression)

#if DN_ASSERT_ENABLED
  // Assertion macro that is only executed when assertions are enabled.
  // Evaluates the expression and triggers fatal error if it fails.
  #define DN_ASSERT(expression) DN_ASSERT_IMPLEMENTATION(expression, #expression)

  // Assertion macro that is always evaluated, even when assertions are disabled.
  // Triggers a fatal error if it fails, but only when assertions are enabled.
  #define DN_ASSERT_EVALUATE(expression) DN_ASSERT_IMPLEMENTATION(expression, #expression)
#else
  #define DN_ASSERT(expression)
  #define DN_ASSERT_EVALUATE(expression) (void)(expression)
#endif // DN_ASSERT_ENABLED
