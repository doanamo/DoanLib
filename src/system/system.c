#include "dn/system.h"
#include "dn/memory.h"
#include <winerror.h>
#include <winnt.h>

// ========================================================================= //

#if DN_PLATFORM_WINDOWS

void DnSysWin32_LogLastError() {
  DWORD error = GetLastError();
  if (error == ERROR_SUCCESS) {
    return;
  }

  DnMemTempScope tempScope = DnMemTemp_PushScope();

  u64 capacity = 256;
  char* buffer = DN_MEM_ALLOC_TYPES(g_dnMemAllocatorTemp, char, capacity);
  DWORD length;

  while (true) {
    length = FormatMessage(
      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr, error, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
      buffer, (DWORD)capacity, nullptr);

    if (length != 0) {
      // Remove trailing newline character.
      char* lastCharacter = &buffer[length - 1];
      if (*lastCharacter == '\n') {
        *lastCharacter = '\0';
      }

      break;
    }

    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
      snprintf(buffer, capacity, "No description available");
      break;
    }

    capacity *= 2;
    buffer = DN_MEM_REALLOC_TYPES(g_dnMemAllocatorTemp, buffer, char, capacity);
  }

  DN_LOG_ERROR("Win32 Error: 0x%08lX - %s", error, buffer);
  DnMemTemp_PopScope(&tempScope);
}

#endif // DN_PLATFORM_WINDOWS
