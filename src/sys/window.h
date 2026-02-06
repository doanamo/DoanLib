#pragma once

typedef void (*DnSysWindowCloseCallback)();

extern DnSysWindowCloseCallback g_dnSysWindowCloseCallback;
extern HWND g_dnSysWindowHandle;
extern int g_dnSysWindowWidth;
extern int g_dnSysWindowHeight;

bool DnSysWindowInit(const char* name, int width, int height);
void DnSysWindowProcessMessages();
void DnSysWindowDeinit();

void DnSysWindowShow();
void DnSysWindowHide();
