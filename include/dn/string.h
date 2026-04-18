#pragma once

#include <string.h>

/*
 * String macros
 */

#define DN_STR_VIEW_FMT "%.*s"
#define DN_STR_VIEW_ARG(view) (view.length), (view.data)
#define DN_STR_VIEW_LITERAL(literal) ((DnStrView){ .data = text, .length = sizeof(literal) - 1 })

/*
 * String view
 */

typedef struct DnStrView {
  const char* data;
  u64 length;
} DnStrView;

static inline DnStrView DnStrView_FromCStr(const char* string) {
  return (DnStrView){
    .data = string,
    .length = strlen(string),
  };
}

static inline DnStrView DnStrView_FromCStrLength(const char* string, u64 length) {
  return (DnStrView){
    .data = string,
    .length = length,
  };
}

static inline bool DnStrView_IsValid(DnStrView view) {
  return view.data != nullptr;
}

static inline bool DnStrView_IsEmpty(DnStrView view) {
  return view.data == nullptr || view.length == 0;
}
