#include "dn/memory.h"
#include "dn/utility.h"

bool DnUtilFile_Read(const DnMemAllocator* allocator, DnStrView path, u8** outData, u64* outSize) {
  DN_ASSERT(allocator);
  DN_ASSERT(outData);
  DN_ASSERT(outSize);

  bool success = false;
  FILE* file = nullptr;
  u8* data = nullptr;

  DnMemTempScope tempScope = DnMemTemp_PushScope();

  const char* pathStr = DnStrView_AsCStr(g_dnMemAllocatorTemp, path);
  file = fopen(pathStr, "rb");
  if (!file) {
    goto error;
  }

  fseek(file, 0, SEEK_END);
  i64 size = ftell(file);
  fseek(file, 0, SEEK_SET);

  if (size < 0) {
    goto error;
  }

  data = DN_MEM_ALLOC(allocator, (u64)size, DnMem_DefaultAlignment);
  u64 read = fread(data, 1, (u64)size, file);

  if (read != (u64)size) {
    goto error;
  }

  success = true;
  *outData = data;
  *outSize = (u64)size;

error:
  if (file) {
    fclose(file);
  }

  if (!success) {
    DN_MEM_FREE(allocator, data);
  }

  DnMemTemp_PopScope(&tempScope);

  return success;
}
