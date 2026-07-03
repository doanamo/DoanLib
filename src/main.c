#include "dn/main.h"
#include "dn/memory.h"

// == MAIN ENTRY ============================================================ //

// Declaration of user defined main entry function.
DnErrorCode DnMain();

DnErrorCode DnMainWrapper(int argc, char* argv[]) {
  // #todo: Parse commandline arguments.
  DN_UNUSED(argc);
  DN_UNUSED(argv);

  DnErrorCode result = DnErrorCode_UnknownFailure;
  if (!DnLib_Init()) {
     result = DnErrorCode_LibInitFailure;
     goto error;
  }

  result = DnMain();

error:
  DnLib_Deinit();
  return result;
}

// == LIBRARY INITIALIZATION ================================================ //

bool DnLib_Init() {
  if (!DnMem_Init()) {
    return false;
  }

  return true;
}

void DnLib_Deinit() {
  DnMem_Deinit();
}
