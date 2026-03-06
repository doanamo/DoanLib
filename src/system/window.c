#include "dn/system.h"
#include <winerror.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct DnSysWindow {
  HWND handle;
  u32 width;
  u32 height;

  DnSysWindowCloseCallback closeCallback;
};

LRESULT CALLBACK DnSysWindow_Procedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  DnSysWindow* window = (DnSysWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

  switch (uMsg) {
  case WM_CREATE: {
      LPCREATESTRUCT createStruct = (LPCREATESTRUCT)lParam;
      window = (DnSysWindow*)createStruct->lpCreateParams;
      SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
    }
    break;

  case WM_CLOSE:
    if (window->closeCallback) {
      window->closeCallback();
      return 0;
    }
    break;

  case WM_EXITSIZEMOVE: {
      DN_ASSERT(window);

      RECT clientSize;
      GetClientRect(window->handle, &clientSize);
      u32 width = (u32)clientSize.right;
      u32 height = (u32)clientSize.bottom;

      if (width != window->width || height != window->height) {
        DN_LOG_INFO("Window resized from %ux%u to %ux%u", window->width, window->height, width, height);
        window->width = width;
        window->height = height;
      }
    }
    break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

DnSysWindow* DnSysWindow_Create() {
  DN_LOG_INFO("Creating system window");
  bool result = false;

  DnSysWindow* window = DN_MEM_ALLOC_TYPE(DnSysWindow);
  *window = (DnSysWindow){
    .width = 1024,
    .height = 576,
  };

  HINSTANCE instance = GetModuleHandle(nullptr);
  if (!instance) {
    DN_LOG_ERROR("Failed to retrieve Win32 instance");
    goto error;
  }

  WNDCLASSEX windowClass = {
    .cbSize = sizeof(WNDCLASSEX),
    .hInstance = instance,
    .lpfnWndProc = DnSysWindow_Procedure,
    .lpszClassName = "DefaultWindowClass",
    .style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
    .hCursor = LoadCursor(nullptr, IDC_ARROW),
    .hIcon = LoadIcon(nullptr, IDI_APPLICATION),
    .hIconSm = LoadIcon(nullptr, IDI_APPLICATION),
  };

  if (RegisterClassEx(&windowClass) == 0) {
    if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
      DN_LOG_ERROR("Failed to register Win32 window class");
      goto error;
    }
  }

  DWORD windowStyle = WS_OVERLAPPEDWINDOW;
  DWORD windowStyleEx = WS_EX_OVERLAPPEDWINDOW;

  RECT clientSize = { 0, 0, (LONG)window->width, (LONG)window->height };
  AdjustWindowRectEx(&clientSize, windowStyle, false, windowStyleEx);

  window->handle = CreateWindowEx(windowStyleEx, windowClass.lpszClassName, "DoanLib Window", windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, clientSize.right - clientSize.left, clientSize.bottom - clientSize.top, nullptr, nullptr, instance, window);

  if (!window->handle) {
    DN_LOG_ERROR("Failed to create Win32 window");
    goto error;
  }

  GetClientRect(window->handle, &clientSize);
  window->width = (u32)clientSize.right;
  window->height = (u32)clientSize.bottom;

  result = true;

error:
  if (!result) {
    DnSysWindow_Destroy(window);
    window = nullptr;
  }

  return window;
}

void DnSysWindow_ProcessMessages(DnSysWindow* window) {
  DN_ASSERT(window->handle);

  MSG message;
  while (PeekMessage(&message, window->handle, 0, 0, PM_REMOVE)) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
}

void DnSysWindow_Destroy(DnSysWindow* window) {
  DN_ASSERT(window);

  if (window->handle) {
    DestroyWindow(window->handle);
  }

  DN_MEM_FREE(window);
}

void DnSysWindow_SetTitle(DnSysWindow* window, const char* title) {
  DN_ASSERT(window->handle);
  SetWindowText(window->handle, title);
}

void DnSysWindow_SetSize(DnSysWindow* window, u32 width, u32 height) {
  DN_ASSERT(window->handle);

  if (width == window->width && height == window->height)
    return;

  DWORD windowStyle = (DWORD)GetWindowLong(window->handle, GWL_STYLE);
  DWORD windowStyleEx = (DWORD)GetWindowLong(window->handle, GWL_EXSTYLE);

  RECT clientSize = { 0, 0, (LONG)width, (LONG)height };
  AdjustWindowRectEx(&clientSize, windowStyle, false, windowStyleEx);

  SetWindowPos(window->handle, nullptr, 0, 0, clientSize.right - clientSize.left, clientSize.bottom - clientSize.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

  DN_LOG_INFO("Window resized from %ux%u to %ux%u", window->width, window->height, width, height);
  window->width = width;
  window->height = height;
}

void DnSysWindow_SetVisibility(DnSysWindow* window, bool visible) {
  DN_ASSERT(window->handle);
  ShowWindow(window->handle, visible ? SW_SHOW : SW_HIDE);
}

void DnSysWindow_SetCloseCallback(DnSysWindow* window, DnSysWindowCloseCallback callback) {
  DN_ASSERT(window->handle);
  window->closeCallback = callback;
}
