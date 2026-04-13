#include "dn/memory.h"

bool DnMemTemp_Init() {
  return true;
}

void DnMemTemp_Deinit() {
}

// #todo: Implement temp bump allocator
// #todo: Replace uses of malloc with temp allocator where appopriate
