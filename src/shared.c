#include "dn/shared.h"
#include <stdio.h>

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
