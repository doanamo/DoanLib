#include "window.h"

void (*g_platform_window_close_callback)() = nullptr;
HWND g_platform_window_handle = nullptr;
int g_platform_window_width = 0;
int g_platform_window_height = 0;

LRESULT CALLBACK platform_window_procedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch(uMsg) {
    case WM_CLOSE:
      if (g_platform_window_close_callback) {
        g_platform_window_close_callback();
        return 0;
      }
      break;

    case WM_EXITSIZEMOVE:
      RECT client_size;
      GetClientRect(g_platform_window_handle, &client_size);
      int new_window_width = (int)client_size.right;
      int new_window_height = (int)client_size.bottom;

      if (new_window_width != g_platform_window_width || new_window_height != g_platform_window_height) {
        LOG_INFO("Window resized from %ix%i to %ix%i", g_platform_window_width, g_platform_window_height, new_window_width, new_window_height);
        g_platform_window_width = new_window_width;
        g_platform_window_height = new_window_height;
      }
      break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool platform_window_init(const char* name, const int width, const int height) {
  LOG_INFO("Initializing platform window");

  HINSTANCE instance = GetModuleHandle(nullptr);
  if (!instance) {
    LOG_ERROR("Failed to retrieve Win32 instance");
    goto error;
  }

  WNDCLASSEX window_class = {
    .cbSize = sizeof(WNDCLASSEX),
    .hInstance = instance,
    .lpfnWndProc = platform_window_procedure,
    .lpszClassName = "DefaultWindowClass",
    .style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
    .hCursor = LoadCursor(nullptr, IDC_ARROW),
    .hIcon = LoadIcon(nullptr, IDI_APPLICATION),
    .hIconSm = LoadIcon(nullptr, IDI_APPLICATION),
  };

  if (RegisterClassEx(&window_class) == 0) {
    LOG_ERROR("Failed to register Win32 window class");
    goto error;
  }

  unsigned window_style = WS_OVERLAPPEDWINDOW;
  unsigned window_style_ex = WS_EX_OVERLAPPEDWINDOW;

  RECT window_size = { 0, 0, width, height };
  AdjustWindowRectEx(&window_size, window_style, false, window_style_ex);

  g_platform_window_handle = CreateWindowEx(window_style_ex, window_class.lpszClassName, name, window_style, CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top, nullptr, nullptr, instance, nullptr);

  if (!g_platform_window_handle) {
    LOG_ERROR("Failed to create Win32 window");
    goto error;
  }

  RECT client_size;
  GetClientRect(g_platform_window_handle, &client_size);
  g_platform_window_width = (int)client_size.right;
  g_platform_window_height = (int)client_size.bottom;
  LOG_INFO("Created %ix%i window", g_platform_window_width, g_platform_window_height);

  return true;

error:
  return false;
}

void platform_window_process_messages() {
  MSG message;
  while (PeekMessage(&message, g_platform_window_handle, 0, 0, PM_REMOVE)) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
}

void platform_window_deinit() {
  LOG_INFO("Deinitializing platform window");

  if (g_platform_window_handle) {
    DestroyWindow(g_platform_window_handle);
    g_platform_window_handle = nullptr;
    g_platform_window_width = 0;
    g_platform_window_height = 0;
  }
}

void platform_window_show() {
  ShowWindow(g_platform_window_handle, SW_SHOW);
}

void platform_window_hide() {
  ShowWindow(g_platform_window_handle, SW_HIDE);
}
