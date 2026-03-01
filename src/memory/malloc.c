#include "dn/memory.h"
#include <stdlib.h>

void* DnMem_Alloc(u64 size) {
  void* allocated = malloc(size);
  DN_ASSERT_ALWAYS(allocated);
  return allocated;
}

void* DnMem_Realloc(void* allocation, u64 size) {
  void* reallocation = realloc(allocation, size);
  DN_ASSERT_ALWAYS(reallocation);
  return reallocation;
}

void DnMem_Free(void* allocation) {
  free(allocation);
}
