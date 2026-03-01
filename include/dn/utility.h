#pragma once

/*
 * File utility
 */

bool DnUtilFile_Read(DnMemAllocator* allocator, const char* path, u8** outData, u64* outSize);
