#pragma once

#include "shared.h"
#include "memory.h"

// == STRING MACROS ========================================================= //

// Printf format string for string view and its size.
// #todo: Create a custom printf format specified string views.
#define DN_STR_VIEW_FMT "%.*s"

// String view arguments for printf format.
#define DN_STR_VIEW_ARG(view) (view.length), (view.data)

// Macros for creating string view from literal. Avoids calling strlen() by
// deducing length at compile time.
#define DN_STR_VIEW_LITERAL(text) ((DnStrView) { .data = text, .length = sizeof(text) - 1 })

// == STRING VIEW STRUCT ==================================================== //

// String view struct represented as pointer to memory (which is not required to
// be null terminated) and length integer.
typedef struct DnStrView {
  const char* data;
  u64 length;
} DnStrView;

// == STRING VIEW FUNCTIONS ================================================= //

// Create a string view from a null-terminated C string.
static inline DnStrView DnStrView_FromCStr(const char* string) {
  return (DnStrView) {
    .data = string,
    .length = strlen(string),
  };
}

// Create a string view from memory and known length.
static inline DnStrView DnStrView_FromCStrLength(const char* string, u64 length) {
  DN_ASSERT(string || length == 0);
  return (DnStrView) {
    .data = string,
    .length = length,
  };
}

// Checks whether string view is empty.
static inline bool DnStrView_IsEmpty(DnStrView view) {
  DN_ASSERT(view.data || view.length == 0);
  return view.data == nullptr || view.length == 0;
}

// Converts string view to a null-terminated C string. This might require
// allocating memory in order to terminate the string.
const char* DnStrView_AsCStr(const DnMemAllocator* allocator, DnStrView view);
