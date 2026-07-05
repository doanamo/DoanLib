#include "dn/memory.h"
#include "dn/system.h"

// == WINDOW STRUCT ========================================================= //

struct DnSysWindow {
  HWND handle;
  u32 width;
  u32 height;

  bool resizing;
  bool closing;

  DnSysWindowResizeCallback resizeCallback;
  void* resizeUserdata;

  DnSysWindowCloseCallback closeCallback;
  void* closeUserdata;
};

// == WINDOW PROCEDURE ====================================================== //

static void DnSysWindow_UpdateSize(DnSysWindow* window) {
  DN_ASSERT(window);

  RECT clientSize;
  GetClientRect(window->handle, &clientSize);
  u32 width = (u32)clientSize.right;
  u32 height = (u32)clientSize.bottom;

  if (width == 0 || height == 0)
    return;

  if (width == window->width && height == window->height)
    return;

  DN_LOG_INFO("Window resized from %ux%u to %ux%u", window->width, window->height, width, height);
  window->width = width;
  window->height = height;

  if (window->resizeCallback) {
    window->resizeCallback(width, height, window->resizeUserdata);
  }
}

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
      window->closing = true;
      window->closeCallback(&window->closing, window->closeUserdata);
      return 0;
    }
    break;

  case WM_SIZE:
    if (!window->resizing) {
      DnSysWindow_UpdateSize(window);
    }
    break;

  case WM_ENTERSIZEMOVE:
    window->resizing = true;
    break;

  case WM_EXITSIZEMOVE:
    window->resizing = false;
    DnSysWindow_UpdateSize(window);
    break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void DnSysWindow_ProcessMessages(DnSysWindow* window) {
  DN_ASSERT(window->handle);

  MSG message;
  while (PeekMessage(&message, window->handle, 0, 0, PM_REMOVE)) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
}

// == WINDOW CREATION ======================================================= //

DnSysWindow* DnSysWindow_Create() {
  DN_LOG_INFO("Creating system window");
  bool success = false;

  DnSysWindow* window = DN_MEM_ALLOC_TYPE(DnMemAllocator_GetDefault(), DnSysWindow);
  *window = (DnSysWindow) {
    .width = 1024,
    .height = 576,
    .resizing = false,
    .closing = false,
  };

  HINSTANCE instance = GetModuleHandle(nullptr);
  if (!instance) {
    DN_LOG_ERROR("Failed to retrieve Win32 instance");
    DnSysWin32_LogLastError();
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
      DnSysWin32_LogLastError();
      goto error;
    }
  }

  DWORD windowStyle = WS_OVERLAPPEDWINDOW;
  DWORD windowStyleEx = WS_EX_OVERLAPPEDWINDOW;

  RECT clientSize = { 0, 0, (LONG)window->width, (LONG)window->height };
  AdjustWindowRectEx(&clientSize, windowStyle, false, windowStyleEx);

  window->handle = CreateWindowEx(windowStyleEx, windowClass.lpszClassName,
    "DoanLib Window", windowStyle, CW_USEDEFAULT, CW_USEDEFAULT,
    clientSize.right - clientSize.left, clientSize.bottom - clientSize.top,
    nullptr, nullptr, instance, window);

  if (!window->handle) {
    DN_LOG_ERROR("Failed to create Win32 window");
    DnSysWin32_LogLastError();
    goto error;
  }

  GetClientRect(window->handle, &clientSize);
  window->width = (u32)clientSize.right;
  window->height = (u32)clientSize.bottom;

  success = true;

error:
  if (!success) {
    DnSysWindow_Destroy(window);
    window = nullptr;
  }

  return window;
}

void DnSysWindow_Destroy(DnSysWindow* window) {
  DN_ASSERT(window);

  if (window->handle) {
    DestroyWindow(window->handle);
  }

  DN_MEM_FREE(DnMemAllocator_GetDefault(), window);
}

// == WINDOW PRESENT ======================================================== //

void DnSysWindow_Present(DnSysWindow* window, const u32* pixels, u32 width, u32 height) {
  DN_ASSERT(window->handle);
  DN_ASSERT(pixels);

  BITMAPV4HEADER bitmapHeader = {0};
  bitmapHeader.bV4Size = sizeof(BITMAPV4HEADER);
  bitmapHeader.bV4Width = (LONG)width;
  bitmapHeader.bV4Height = -(LONG)height;
  bitmapHeader.bV4Planes = 1;
  bitmapHeader.bV4BitCount = 32;
  bitmapHeader.bV4V4Compression = BI_BITFIELDS;
  bitmapHeader.bV4RedMask = 0x000000FF;
  bitmapHeader.bV4GreenMask = 0x0000FF00;
  bitmapHeader.bV4BlueMask = 0x00FF0000;
  bitmapHeader.bV4AlphaMask = 0xFF000000;

  HDC deviceContext = GetDC(window->handle);
  StretchDIBits(deviceContext, 0, 0, (int)window->width, (int)window->height,
    0, 0, (int)width, (int)height, pixels, (BITMAPINFO*)&bitmapHeader,
    DIB_RGB_COLORS, SRCCOPY);
  ReleaseDC(window->handle, deviceContext);
}

// == WINDOW SETTERS ======================================================== //

void DnSysWindow_SetTitle(DnSysWindow* window, DnStrView title) {
  DN_ASSERT(window->handle);

  DnMemTempScope memTempScope = DnMemTemp_PushScope();
  const char* titleStr = DnStrView_AsCStr(DnMemTemp_GetAllocator(), title);
  SetWindowText(window->handle, titleStr);
  DnMemTemp_PopScope(&memTempScope);
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

void DnSysWindow_SetResizeCallback(DnSysWindow* window, DnSysWindowResizeCallback callback, void* userdata) {
  DN_ASSERT(window->handle);
  window->resizeCallback = callback;
  window->resizeUserdata = userdata;
}

void DnSysWindow_SetCloseCallback(DnSysWindow* window, DnSysWindowCloseCallback callback, void* userdata) {
  DN_ASSERT(window->handle);
  window->closeCallback = callback;
  window->closeUserdata = userdata;
}

// == WINDOW GETTERS ======================================================== //

bool DnSysWindow_IsClosing(DnSysWindow* window) {
  return window->closing;
}

HWND DnSysWindow_GetHandle(DnSysWindow* window) {
  DN_ASSERT(window->handle);
  return window->handle;
}
