#pragma once

/*
 * System window
 */

typedef struct DnSysWindow DnSysWindow;
typedef void (*DnSysWindowCloseCallback)();

DnSysWindow* DnSysWindow_Create();
void DnSysWindow_ProcessMessages(DnSysWindow* window);
void DnSysWindow_Destroy(DnSysWindow* window);

void DnSysWindow_SetTitle(DnSysWindow* window, const char* title);
void DnSysWindow_SetSize(DnSysWindow* window, u32 width, u32 height);
void DnSysWindow_SetVisibility(DnSysWindow* window, bool visible);
void DnSysWindow_SetCloseCallback(DnSysWindow* window, DnSysWindowCloseCallback callback);
