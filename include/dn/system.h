#pragma once

#include "shared.h"
#include "structs.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// == SYSTEM WINDOW ========================================================= //

// Opaque handle for system window.
typedef struct DnSysWindow DnSysWindow;

// Function declaration for windows resize callback.
typedef void (*DnSysWindowResizeCallback)(i32 width, i32 height, void* userdata);

// Function declaration for window close callback.
typedef void (*DnSysWindowCloseCallback)(bool* closing, void* userdata);

// Creates an application window. Window is initially created as hidden.
DnSysWindow* DnSysWindow_Create();

// Processes pending window messages.
void DnSysWindow_ProcessMessages(DnSysWindow* window);

// Closes and destroys the window.
void DnSysWindow_Destroy(DnSysWindow* window);

// Presents pixels array on the window surface.
void DnSysWindow_Present(DnSysWindow* window, const u32* pixels, i32 width, i32 height);

// Sets the title of the window.
void DnSysWindow_SetTitle(DnSysWindow* window, DnStrView title);

// Sets the size of the window's client area, which might be smaller than the
// window's actual size.
void DnSysWindow_SetSize(DnSysWindow* window, i32 width, i32 height);

// Sets the visibility of the window.
void DnSysWindow_SetVisibility(DnSysWindow* window, bool visible);

// Sets the resize callback for the window.
void DnSysWindow_SetResizeCallback(DnSysWindow* window, DnSysWindowResizeCallback callback, void* userdata);

// Sets the close callback for the window.
void DnSysWindow_SetCloseCallback(DnSysWindow* window, DnSysWindowCloseCallback callback, void* userdata);

// Returns whether the window is currently closing.
bool DnSysWindow_IsClosing(DnSysWindow* window);

// Returns the width of the window's client area.
i32 DnSysWindow_GetWidth(DnSysWindow* window);

// Returns the height of the window's client area.
i32 DnSysWindow_GetHeight(DnSysWindow* window);

// Returns Win32 handle of the window.
HWND DnSysWindow_GetHandle(DnSysWindow* window);

// == PLATFORM SPECIFIC ===================================================== //

#if DN_PLATFORM_WINDOWS
  // Logs the last error from the Windows API if an error has occurred. Does not
  // do anything if no error has occurred.
  void DnSysWin32_LogLastError();
#endif
