#include "dn/memory.h"
#include "dn/structs.h"

// #todo: Implement string object

// == STRING VIEW FUNCTIONS ================================================= //

const char* DnStrView_AsCStr(const DnMemAllocator* allocator, DnStrView view) {
  DN_ASSERT(allocator);

  char* result = (char*)DN_MEM_ALLOC(allocator, view.length + 1, DnMem_DefaultAlignment);
  if (!DnStrView_IsEmpty(view)) {
    memcpy(result, view.data, view.length);
  }

  result[view.length] = '\0';
  return result;
}
