#include "dn/main.h"
#include "dn/memory.h"

// == MAIN ENTRY ============================================================ //

// Declaration of user defined main entry function.
DnExitCode DnMain();

DnExitCode DnMainWrapper(int argc, char* argv[]) {
  // #todo: Parse commandline arguments.
  DN_UNUSED(argc);
  DN_UNUSED(argv);

  DnExitCode result = DnExitCode_UnknownFailure;
  if (!DnLib_Init()) {
     result = DnExitCode_LibInitFailure;
     goto error;
  }

  DnMemTempScope tempScope = DnMemTemp_PushScope();
  result = DnMain();
  DnMemTemp_PopScope(&tempScope);

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
