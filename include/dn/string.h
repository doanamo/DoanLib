#pragma once

#include <string.h>

/*
 * String macros
 */

#define DN_STRING_VIEW_FMT "%.*s"
#define DN_STRING_VIEW_ARG(view) (view.length), (view.data)
#define DN_STRING_VIEW_LITERAL(text) ((DnStringView){ .data = text, .length = sizeof(text) - 1 })

/*
 * String view
 */

typedef struct DnStringView {
  const char* data;
  size_t length;
} DnStringView;

static inline DnStringView DnStringView_FromCString(const char* string) {
  return (DnStringView){
    .data = string,
    .length = strlen(string),
  };
}

static inline DnStringView DnStringView_FromCStringLength(const char* string, size_t length) {
  return (DnStringView){
    .data = string,
    .length = length,
  };
}

static inline bool DnStringView_IsValid(DnStringView view) {
  return view.data != nullptr;
}

static inline bool DnStringView_IsEmpty(DnStringView view) {
  return view.data == nullptr || view.length == 0;
}
