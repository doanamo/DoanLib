#pragma once

typedef void (*SysWindowCloseCallback)();

extern SysWindowCloseCallback g_sysWindowCloseCallback;
extern HWND g_sysWindowHandle;
extern int g_sysWindowWidth;
extern int g_sysWindowHeight;

bool SysWindowInit(const char* name, int width, int height);
void SysWindowProcessMessages();
void SysWindowDeinit();

void SysWindowShow();
void SysWindowHide();
