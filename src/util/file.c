#include "dn/shared.h"
#include "dn/util/file.h"

bool DnFileRead(DnMemAllocator* allocator, const char* path, uint8_t** outData, size_t* outSize) {
  DN_UNUSED(allocator != nullptr);
  DN_ASSERT(path != nullptr);
  DN_ASSERT(outData != nullptr);
  DN_ASSERT(outSize != nullptr);

  bool result = false;
  FILE* file = nullptr;
  uint8_t* data = nullptr;

  file = fopen(path, "rb");
  if (!file) {
    goto error;
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  if (size < 0) {
    goto error;
  }

  data = (uint8_t*)allocator->alloc(allocator, (size_t)size);
  size_t read = fread(data, 1, (size_t)size, file);

  if (read != (size_t)size) {
    goto error;
  }

  result = true;
  *outData = data;
  *outSize = (size_t)size;

error:
  if (file) {
    fclose(file);
  }

  if (!result && data) {
    allocator->free(allocator, data);
  }

  return result;
}
