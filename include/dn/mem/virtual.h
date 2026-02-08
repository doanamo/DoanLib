#pragma once

u32 DnMemVirtualPageSize();
void* DnMemVirtualReserve(u64 size);
bool DnMemVirtualCommit(void* ptr, u64 size);
bool DnMemVirtualDecommit(void* ptr, u64 size);
bool DnMemVirtualRelease(void* ptr);
