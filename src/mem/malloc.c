#include "dn/mem/malloc.h"

void* DnMemAlloc(u64 size) {
  void* allocated = malloc(size);
  DN_ASSERT_ALWAYS(allocated);
  return allocated;
}

void* DnMemRealloc(void* allocation, u64 size) {
  void* reallocation = realloc(allocation, size);
  DN_ASSERT_ALWAYS(reallocation);
  return reallocation;
}

void DnMemFree(void* allocation) {
  free(allocation);
}
