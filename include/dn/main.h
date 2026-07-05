#pragma once

// == EXIT CODES =========================================================== //

// List of known exit codes returned by the library.
typedef enum DnExitCode : int {
  DnExitCode_Success = 0,
  DnExitCode_UnknownFailure = 1,
  DnExitCode_LibInitFailure = 2,
  DnExitCode_AppInitFailure = 3,
} DnExitCode;

// == MAIN ENTRY =========================================================== //

// Helps define the main entry point for the library. This is for convenience
// and for aid in correctness of initialization and deinitialization.
#define DN_DEFINE_MAIN_ENTRY() \
  DnExitCode DnMainWrapper(int argc, char* argv[]); \
  int main(int argc, char* argv[]) { \
    return DnMainWrapper(argc, argv); \
  } \
  DnExitCode DnMain()

// == LIBRARY INITITIALIZATION ============================================= //

// Initializes the library. Should be called before any other library functions
// at the very beginning of main().
bool DnLib_Init();

// Deinitializes the library. Should be called after all other library functions
// have been called at the very end of main().
void DnLib_Deinit();
