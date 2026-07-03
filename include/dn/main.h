#pragma once

// == ERROR CODES ========================================================== //

// List of known error codes returned by the library.
typedef enum DnErrorCode : int {
  DnErrorCode_Success = 0,
  DnErrorCode_UnknownFailure = 1,
  DnErrorCode_LibInitFailure = 2,
} DnErrorCode;

// == MAIN ENTRY =========================================================== //

// Helps define the main entry point for the library. This is for convenience
// and for aid in correctness of initialization and deinitialization.
#define DN_DEFINE_MAIN_ENTRY() \
  DnErrorCode DnMainWrapper(int argc, char* argv[]); \
  int main(int argc, char* argv[]) { \
    return DnMainWrapper(argc, argv); \
  } \
  DnErrorCode DnMain()

// == LIBRARY INITITIALIZATION ============================================= //

// Initializes the library. Should be called before any other library functions
// at the very beginning of main().
bool DnLib_Init();

// Deinitializes the library. Should be called after all other library functions
// have been called at the very end of main().
void DnLib_Deinit();
