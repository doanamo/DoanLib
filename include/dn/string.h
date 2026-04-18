#pragma once

#include <string.h>

/*
 * String macros
 */

#define DN_STR_VIEW_FMT "%.*s"
#define DN_STR_VIEW_ARG(view) (view.length), (view.data)
#define DN_STR_VIEW_LITERAL(text) ((DnStrView){ .data = text, .length = sizeof(text) - 1 })

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
  DN_ASSERT(string != nullptr || length == 0);
  return (DnStrView){
    .data = string,
    .length = length,
  };
}

static inline bool DnStrView_IsEmpty(DnStrView view) {
  DN_ASSERT(view.data != nullptr || view.length == 0);
  return view.data == nullptr || view.length == 0;
}

const char* DnStrView_AsCStr(const DnMemAllocator* allocator, DnStrView view);
