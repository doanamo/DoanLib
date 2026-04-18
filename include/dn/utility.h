#pragma once

/*
 * File utility
 */

bool DnUtilFile_Read(const DnMemAllocator* allocator, DnStrView path, u8** outData, u64* outSize);
