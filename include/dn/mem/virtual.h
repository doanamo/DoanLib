#pragma once

u64 DnMemVirtualPageSize();
void* DnMemVirtualReserve(u64 size);
bool DnMemVirtualCommit(void* ptr, u64 size);
void DnMemVirtualDecommit(void* ptr, u64 size);
void DnMemVirtualRelease(void* ptr);
