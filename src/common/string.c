#include "dn/string.h"

// #todo: Implement string object

const char* DnStrView_AsCStr(const DnMemAllocator* allocator, DnStrView view) {
  DN_ASSERT(allocator);

  char* result = (char*)DN_MEM_ALLOC(allocator, view.length + 1);
  if (!DnStrView_IsEmpty(view))
  {
    memcpy(result, view.data, view.length);
  }

  result[view.length] = '\0';
  return result;
}
