#pragma once

void* DnMem_Alloc(u64 size);
void* DnMem_Realloc(void* allocation, u64 size);
void DnMem_Free(void* allocation);
