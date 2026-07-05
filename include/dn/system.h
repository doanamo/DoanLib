#pragma once

#include "shared.h"
#include "structs.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// == SYSTEM WINDOW ========================================================= //

// Opaque handle for system window.
typedef struct DnSysWindow DnSysWindow;

// Function declaration for window close callback.
typedef void (*DnSysWindowCloseCallback)(void* userdata);

// Creates an application window. Window is initially created as hidden.
DnSysWindow* DnSysWindow_Create();

// Processes pending window messages.
void DnSysWindow_ProcessMessages(DnSysWindow* window);

// Closes and destroys the window.
void DnSysWindow_Destroy(DnSysWindow* window);

// Sets the title of the window.
void DnSysWindow_SetTitle(DnSysWindow* window, DnStrView title);

// Sets the size of the window's client area, which might be smaller than the
// window's actual size.
void DnSysWindow_SetSize(DnSysWindow* window, u32 width, u32 height);

// Sets the visibility of the window.
void DnSysWindow_SetVisibility(DnSysWindow* window, bool visible);

// Sets the close callback for the window.
void DnSysWindow_SetCloseCallback(DnSysWindow* window, DnSysWindowCloseCallback callback, void* userdata);

// Returns Win32 handle of the window.
HWND DnSysWindow_GetHandle(DnSysWindow* window);

// == PLATFORM SPECIFIC ===================================================== //

#if DN_PLATFORM_WINDOWS
  // Logs the last error from the Windows API if an error has occurred. Does not
  // do anything if no error has occurred.
  void DnSysWin32_LogLastError();
#endif
