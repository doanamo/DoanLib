#include "dn/shared.h"

static_assert(sizeof(i8) == 1);
static_assert(sizeof(i16) == 2);
static_assert(sizeof(i32) == 4);
static_assert(sizeof(i64) == 8);
static_assert(sizeof(u8) == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);
static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);

#if DN_LOG_ENABLED

void DnLog_Info(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stdout, format, args);
  fflush(stdout);
  va_end(args);
}

void DnLog_Error(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  fflush(stdout);
  fflush(stderr);
  va_end(args);
}

#endif
