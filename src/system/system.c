#include "dn/system.h"
#include <winerror.h>
#include <winnt.h>

// == WINDOWS SPECIFIC FUNCTIONS ============================================ //

#if DN_PLATFORM_WINDOWS

void DnSysWin32_LogLastError() {
  DWORD error = GetLastError();
  if (error == ERROR_SUCCESS) {
    return;
  }

  // Note: Temporary allocator is not used here instead, because it may not be
  // initialized yet or it may have already been destroyed.

  LPSTR buffer = nullptr;
  DWORD length = FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    nullptr, error, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPTSTR)&buffer, 0, nullptr);

  if (length != 0) {
    // Remove trailing newline character.
    char* lastCharacter = &buffer[length - 1];
    if (*lastCharacter == '\n') {
      *lastCharacter = '\0';
    }

    DN_LOG_ERROR("Win32 Error: 0x%08lX - %s", error, buffer);
    LocalFree(buffer);
  }
  else {
    DN_LOG_ERROR("Win32 Error: 0x%08lX - Unknown.", error);
  }
}

#endif // DN_PLATFORM_WINDOWS
