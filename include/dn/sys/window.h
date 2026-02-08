#pragma once

typedef void (*DnSysWindowCloseCallback)();

extern DnSysWindowCloseCallback g_dnSysWindowCloseCallback;
extern HWND g_dnSysWindowHandle;
extern u32 g_dnSysWindowWidth;
extern u32 g_dnSysWindowHeight;

bool DnSysWindowInit(const char* name, u32 width, u32 height);
void DnSysWindowProcessMessages();
void DnSysWindowDeinit();

void DnSysWindowShow();
void DnSysWindowHide();
