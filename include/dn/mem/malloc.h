#pragma once

void* DnMemAlloc(u64 size);
void* DnMemRealloc(void* allocation, u64 size);
void DnMemFree(void* allocation);
