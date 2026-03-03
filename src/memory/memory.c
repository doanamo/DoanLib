#include "dn/memory.h"

bool DnMemAllocators_Init();
void DnMemAllocators_Deinit();

bool DnMem_Init() {
  if (!DnMemAllocators_Init()) {
    return false;
  }

  return true;
}

void DnMem_Deinit() {
  DnMemAllocators_Deinit();
}
