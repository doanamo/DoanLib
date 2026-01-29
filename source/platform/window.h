#pragma once

typedef void (*platform_window_close_function_t)();

extern platform_window_close_function_t g_platform_window_close_callback;
extern HWND g_platform_window_handle;
extern int g_platform_window_width;
extern int g_platform_window_height;

bool platform_window_init(const char* name, int width, int height);
void platform_window_process_messages();
void platform_window_deinit();

void platform_window_show();
void platform_window_hide();
