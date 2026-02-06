#include "shared.h"
#include "window.h"

void (*g_sysWindowCloseCallback)() = nullptr;
HWND g_sysWindowHandle = nullptr;
int g_sysWindowWidth = 0;
int g_sysWindowHeight = 0;

LRESULT CALLBACK SysWindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_CLOSE:
    if (g_sysWindowCloseCallback) {
      g_sysWindowCloseCallback();
      return 0;
    }
    break;

  case WM_EXITSIZEMOVE:
    RECT clientSize;
    GetClientRect(g_sysWindowHandle, &clientSize);
    int newWindowWidth = (int)clientSize.right;
    int newWindowHeight = (int)clientSize.bottom;

    if (newWindowWidth != g_sysWindowWidth || newWindowHeight != g_sysWindowHeight) {
      LOG_INFO("Window resized from %ix%i to %ix%i", g_sysWindowWidth, g_sysWindowHeight, newWindowWidth, newWindowHeight);
      g_sysWindowWidth = newWindowWidth;
      g_sysWindowHeight = newWindowHeight;
    }
    break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool SysWindowInit(const char* name, const int width, const int height) {
  LOG_INFO("Initializing system window");

  HINSTANCE instance = GetModuleHandle(nullptr);
  if (!instance) {
    LOG_ERROR("Failed to retrieve Win32 instance");
    goto error;
  }

  WNDCLASSEX windowClass = {
    .cbSize = sizeof(WNDCLASSEX),
    .hInstance = instance,
    .lpfnWndProc = SysWindowProcedure,
    .lpszClassName = "DefaultWindowClass",
    .style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
    .hCursor = LoadCursor(nullptr, IDC_ARROW),
    .hIcon = LoadIcon(nullptr, IDI_APPLICATION),
    .hIconSm = LoadIcon(nullptr, IDI_APPLICATION),
  };

  if (RegisterClassEx(&windowClass) == 0) {
    LOG_ERROR("Failed to register Win32 window class");
    goto error;
  }

  unsigned windowStyle = WS_OVERLAPPEDWINDOW;
  unsigned windowStyleEx = WS_EX_OVERLAPPEDWINDOW;

  RECT windowSize = {0, 0, width, height};
  AdjustWindowRectEx(&windowSize, windowStyle, false, windowStyleEx);

  g_sysWindowHandle = CreateWindowEx(windowStyleEx, windowClass.lpszClassName, name, windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top, nullptr, nullptr, instance, nullptr);

  if (!g_sysWindowHandle) {
    LOG_ERROR("Failed to create Win32 window");
    goto error;
  }

  RECT clientSize;
  GetClientRect(g_sysWindowHandle, &clientSize);
  g_sysWindowWidth = (int)clientSize.right;
  g_sysWindowHeight = (int)clientSize.bottom;
  LOG_INFO("Created %ix%i window", g_sysWindowWidth, g_sysWindowHeight);

  return true;

error:
  return false;
}

void SysWindowProcessMessages() {
  MSG message;
  while (PeekMessage(&message, g_sysWindowHandle, 0, 0, PM_REMOVE)) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
}

void SysWindowDeinit() {
  LOG_INFO("Deinitializing system window");

  if (g_sysWindowHandle) {
    DestroyWindow(g_sysWindowHandle);
    g_sysWindowHandle = nullptr;
    g_sysWindowWidth = 0;
    g_sysWindowHeight = 0;
  }
}

void SysWindowShow() {
  ShowWindow(g_sysWindowHandle, SW_SHOW);
}

void SysWindowHide() {
  ShowWindow(g_sysWindowHandle, SW_HIDE);
}
