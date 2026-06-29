#pragma once

#include "shared.h"
#include "structs.h"

// == FILE UTILITY ========================================================== //

// Reads the contents of a file and returns allocated memory that contains data.
bool DnUtilFile_Read(const DnMemAllocator* allocator, DnStrView path, u8** outData, u64* outSize);
