#include "dn/utility.h"
#include <stdio.h>

bool DnUtilFile_Read(const DnMemAllocator* allocator, const char* path, u8** outData, u64* outSize) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(path != nullptr);
  DN_ASSERT(outData != nullptr);
  DN_ASSERT(outSize != nullptr);

  bool result = false;
  FILE* file = nullptr;
  u8* data = nullptr;

  file = fopen(path, "rb");
  if (!file) {
    goto error;
  }

  fseek(file, 0, SEEK_END);
  i64 size = ftell(file);
  fseek(file, 0, SEEK_SET);

  if (size < 0) {
    goto error;
  }

  data = DN_MEM_ALLOCATOR_REALLOC(allocator, data, u8, (u64)size);
  u64 read = fread(data, 1, (u64)size, file);

  if (read != (u64)size) {
    goto error;
  }

  result = true;
  *outData = data;
  *outSize = (u64)size;

error:
  if (file) {
    fclose(file);
  }

  if (!result && data) {
    DN_MEM_ALLOCATOR_FREE(allocator, data);
  }

  return result;
}
