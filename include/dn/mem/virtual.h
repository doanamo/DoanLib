#pragma once

u64 DnMemVirtual_GetPageSize();
void* DnMemVirtual_Reserve(u64 size);
bool DnMemVirtual_Commit(void* ptr, u64 size);
void DnMemVirtual_Decommit(void* ptr, u64 size);
void DnMemVirtual_Release(void* ptr);
