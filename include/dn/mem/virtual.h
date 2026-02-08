#pragma once

uint32_t DnMemVirtualPageSize();
void* DnMemVirtualReserve(size_t size);
bool DnMemVirtualCommit(void* ptr, size_t size);
bool DnMemVirtualDecommit(void* ptr, size_t size);
bool DnMemVirtualRelease(void* ptr);
