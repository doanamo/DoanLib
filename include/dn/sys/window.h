#pragma once

typedef void (*DnSysWindowCloseCallback)();

extern DnSysWindowCloseCallback g_dnSysWindowCloseCallback;
extern HWND g_dnSysWindowHandle;
extern u32 g_dnSysWindowWidth;
extern u32 g_dnSysWindowHeight;

bool DnSysWindow_Init(const char* name, u32 width, u32 height);
void DnSysWindow_ProcessMessages();
void DnSysWindow_Deinit();

void DnSysWindow_Show();
void DnSysWindow_Hide();
