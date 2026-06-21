#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "shared.h"
#include "structs.h"

// ========================================================================== //
//   System Initialization                                                     //
// ========================================================================== //

/*
 * Configuration structure for system initialization.
 */
typedef struct DnSysConfig {
  DnStrView windowTitle;
  u32 windowWidth;
  u32 windowHeight;
} DnSysConfig;

// ========================================================================== //
//   System Window                                                            //
// ========================================================================== //

typedef struct DnSysWindow DnSysWindow;
typedef void (*DnSysWindowCloseCallback)();

DnSysWindow* DnSysWindow_Create();
void DnSysWindow_ProcessMessages(DnSysWindow* window);
void DnSysWindow_Destroy(DnSysWindow* window);

void DnSysWindow_SetTitle(DnSysWindow* window, DnStrView title);
void DnSysWindow_SetSize(DnSysWindow* window, u32 width, u32 height);
void DnSysWindow_SetVisibility(DnSysWindow* window, bool visible);
void DnSysWindow_SetCloseCallback(DnSysWindow* window, DnSysWindowCloseCallback callback);
HWND DnSysWindow_GetHandle(DnSysWindow* window);

// ========================================================================== //
//   Windows Specific                                                         //
// ========================================================================== //

#if DN_PLATFORM_WINDOWS
  /*
   * Logs the last error from the Windows API if an error has occurred. Does not
   * do anything if no error has occurred.
   */
  void DnSysWin32_LogLastError();
#endif
