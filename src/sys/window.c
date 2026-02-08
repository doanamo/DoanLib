#include "dn/shared.h"
#include "dn/sys/window.h"

DnSysWindowCloseCallback g_dnSysWindowCloseCallback = nullptr;
HWND g_dnSysWindowHandle = nullptr;
u32 g_dnSysWindowWidth = 0;
u32 g_dnSysWindowHeight = 0;

LRESULT CALLBACK DnWindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_CLOSE:
    if (g_dnSysWindowCloseCallback) {
      g_dnSysWindowCloseCallback();
      return 0;
    }
    break;

  case WM_EXITSIZEMOVE:
    RECT clientSize;
    GetClientRect(g_dnSysWindowHandle, &clientSize);
    u32 newWindowWidth = (u32)clientSize.right;
    u32 newWindowHeight = (u32)clientSize.bottom;

    if (newWindowWidth != g_dnSysWindowWidth || newWindowHeight != g_dnSysWindowHeight) {
      DN_LOG_INFO("Window resized from %ix%i to %ix%i", g_dnSysWindowWidth, g_dnSysWindowHeight, newWindowWidth, newWindowHeight);
      g_dnSysWindowWidth = newWindowWidth;
      g_dnSysWindowHeight = newWindowHeight;
    }
    break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool DnSysWindowInit(const char* name, u32 width, u32 height) {
  DN_LOG_INFO("Initializing system window");

  HINSTANCE instance = GetModuleHandle(nullptr);
  if (!instance) {
    DN_LOG_ERROR("Failed to retrieve Win32 instance");
    return false;
  }

  WNDCLASSEX windowClass = {
    .cbSize = sizeof(WNDCLASSEX),
    .hInstance = instance,
    .lpfnWndProc = DnWindowProcedure,
    .lpszClassName = "DefaultWindowClass",
    .style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
    .hCursor = LoadCursor(nullptr, IDC_ARROW),
    .hIcon = LoadIcon(nullptr, IDI_APPLICATION),
    .hIconSm = LoadIcon(nullptr, IDI_APPLICATION),
  };

  if (RegisterClassEx(&windowClass) == 0) {
    DN_LOG_ERROR("Failed to register Win32 window class");
    return false;
  }

  DWORD windowStyle = WS_OVERLAPPEDWINDOW;
  DWORD windowStyleEx = WS_EX_OVERLAPPEDWINDOW;

  RECT windowSize = { 0, 0, width, height };
  AdjustWindowRectEx(&windowSize, windowStyle, false, windowStyleEx);

  DN_ASSERT(g_dnSysWindowHandle == nullptr);
  g_dnSysWindowHandle = CreateWindowEx(windowStyleEx, windowClass.lpszClassName, name, windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top, nullptr, nullptr, instance, nullptr);

  if (!g_dnSysWindowHandle) {
    DN_LOG_ERROR("Failed to create Win32 window");
    return false;
  }

  RECT clientSize;
  GetClientRect(g_dnSysWindowHandle, &clientSize);
  g_dnSysWindowWidth = (u32)clientSize.right;
  g_dnSysWindowHeight = (u32)clientSize.bottom;
  DN_LOG_INFO("Created %ix%i window", g_dnSysWindowWidth, g_dnSysWindowHeight);

  return true;
}

void DnSysWindowProcessMessages() {
  MSG message;
  while (PeekMessage(&message, g_dnSysWindowHandle, 0, 0, PM_REMOVE)) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
}

void DnSysWindowDeinit() {
  DN_LOG_INFO("Deinitializing system window");

  if (g_dnSysWindowHandle) {
    DestroyWindow(g_dnSysWindowHandle);
    g_dnSysWindowHandle = nullptr;
  }

  g_dnSysWindowWidth = 0;
  g_dnSysWindowHeight = 0;
}

void DnSysWindowShow() {
  DN_ASSERT(g_dnSysWindowHandle != nullptr);
  ShowWindow(g_dnSysWindowHandle, SW_SHOW);
}

void DnSysWindowHide() {
  DN_ASSERT(g_dnSysWindowHandle != nullptr);
  ShowWindow(g_dnSysWindowHandle, SW_HIDE);
}
