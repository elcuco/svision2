#include <windows.h>
#include <iostream>

#include "platformwin32.h"
#include "spdlog/spdlog.h"

static const uint8_t WIN32_KEYCODES[] = {
    0,
    27,
    49,
    50,
    51,
    52,
    53,
    54,
    55,
    56,
    57,
    48,
    45,
    61,
    8,
    9,
    81,
    87,
    69,
    82,
    84,
    89,
    85,
    73,
    79,
    80,
    91,
    93,
    10,
    0,
    65,
    83,
    68,
    70,
    71,
    72,
    74,
    75,
    76,
    59,
    39,
    96,
    0,
    92,
    90,
    88,
    67,
    86,
    66,
    78,
    77,
    44,
    46,
    47,
    0,
    0,
    0,
    32,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    17,
    3,
    0,
    20,
    0,
    19,
    0,
    5,
    18,
    4,
    26,
    127};

auto convert_win32_mouse_event(UINT msg, WPARAM wParam, LPARAM lParam) -> EventMouse
{
      auto event = EventMouse();
      if (msg == WM_LBUTTONDOWN)
      {
            event.pressed = true;
            if (wParam & MK_LBUTTON)
            {
                  event.button = 1;
            }
            if (wParam & MK_RBUTTON)
            {
                  event.button = 2;
            }
            if (wParam & MK_MBUTTON)
            {
                  event.button = 3;
            }
      }
      if (msg == WM_MOUSEMOVE)
      {
            event.y = HIWORD(lParam);
            event.x = LOWORD(lParam);
      }
      return event;
}

auto convert_win32_keyboard_event(UINT msg, WPARAM wParam, LPARAM lParam) -> EventKeyboard
{
      auto event = EventKeyboard();
      event.modifiers = ((GetKeyState(VK_CONTROL) & 0x8000) >> 15) |
                        ((GetKeyState(VK_SHIFT) & 0x8000) >> 14) |
                        ((GetKeyState(VK_MENU) & 0x8000) >> 13) |
                        (((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000) >> 12);
      event.key = WIN32_KEYCODES[HIWORD(lParam) & 0x1ff];
      event.keydown = !((lParam >> 31) & 1);
      return event;
}

auto convert_win32_resize_event(UINT msg, WPARAM wParam, LPARAM lParam) -> EventResize
{
      auto event = EventResize();

      switch (msg)
      {
      case WM_MOVE:
            event.position.x = LOWORD(lParam);
            event.position.y = HIWORD(lParam);
            break;

      case WM_MOVING:
            break;

      case WM_SIZE:
            event.size.width = LOWORD(lParam);
            event.size.height = HIWORD(lParam);
            break;

      case WM_SIZING:
            break;

      default:
            break;
      }
      return event;
}

struct PlatformWindowWin32 : public PlatformWindow
{
      HWND hwnd;
};

static auto win32_paint_window(PlatformWindowWin32 *window) -> void
{
      if (window->content.buf == nullptr || window->content.size.height <= 0 || window->content.size.height <= 0)
      {
            spdlog::warn("win32_paint_window: Window has invalid size! ptr={}, size={}x{}", fmt::ptr(window->content.buf), window->content.size.width, window->content.size.height);
            return;
      }

      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(window->hwnd, &ps);
      HDC memdc = CreateCompatibleDC(hdc);
      HBITMAP hbmp = CreateCompatibleBitmap(hdc, window->content.size.width, window->content.size.height);
      HGDIOBJ oldbmp = SelectObject(memdc, hbmp);
      BITMAPINFO bi = {{sizeof(bi), window->content.size.width, -window->content.size.height, 1, 32, BI_RGB}};
      bi.bmiColors[0].rgbRed = 0xff;
      bi.bmiColors[1].rgbGreen = 0xff;
      bi.bmiColors[2].rgbBlue = 0xff;

      SetDIBitsToDevice(memdc, 0, 0, window->content.size.width, window->content.size.height, 0, 0, 0, window->content.size.height,
                        window->content.buf, (BITMAPINFO *)&bi, DIB_RGB_COLORS);
      BitBlt(hdc, 0, 0, window->content.size.width, window->content.size.height, memdc, 0, 0, SRCCOPY);
      SelectObject(memdc, oldbmp);
      DeleteObject(hbmp);
      DeleteDC(memdc);
      EndPaint(window->hwnd, &ps);
}

static LRESULT CALLBACK svision_wndproc(HWND hwnd, UINT msg, WPARAM wParam,
                                        LPARAM lParam)
{
      PlatformWindowWin32 *window = (struct PlatformWindowWin32 *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

      if (window == nullptr)
      {
            spdlog::critical("Window =({}), message={}: is not managed by us, ignoring", (long long)hwnd, msg);
            return DefWindowProcW(hwnd, msg, wParam, lParam);
      }

      auto title = window->title;
      spdlog::debug("Sending message {} to \"{}\"", msg, title);

      switch (msg)
      {
      case WM_PAINT:
            window->draw();
            win32_paint_window(window);
            break;

      case WM_CLOSE:
            if (window->can_close())
            {
                  DestroyWindow(hwnd);
            }
            break;
      case WM_DESTROY:
            PostQuitMessage(0);
            break;

            //      case WM_MOVE:
            //      case WM_MOVING:
      case WM_SIZE:
            //      case WM_SIZING:
            {
                  auto event = convert_win32_resize_event(msg, wParam, lParam);
                  if (event.size != window->content.size)
                  {
                        window->needs_redraw = true;
                        window->content.resize(event.size.width, event.size.height);
                        window->on_resize(event);
                  }
            }
            break;
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      case WM_MOUSEMOVE:
            window->on_mouse(convert_win32_mouse_event(msg, wParam, lParam));
            break;

      case WM_KEYDOWN:
      case WM_KEYUP:
            window->on_keyboard(convert_win32_keyboard_event(msg, wParam, lParam));
            break;
      default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
      }

      if (window->needs_redraw)
      {
            InvalidateRect(window->hwnd, 0, 1);
      }
      return 0;
}

PlatformWin32::PlatformWin32()
{
}

auto PlatformWin32::init() -> void
{
      spdlog::set_level(spdlog::level::info);
      spdlog::info("PlatformWin32 initialized");
}

auto PlatformWin32::done() -> void
{
}

auto PlatformWin32::main_loop() -> void
{
      MSG msg;
      while (GetMessage(&msg, NULL, 0, 0) && !this->exit_loop)
      {
            if (msg.message == WM_QUIT)
                  return;
            TranslateMessage(&msg);
            DispatchMessage(&msg);

#if 0
            if (this->close_on_last_window && this->windows.size() == 0)
            {
                  this->exit_loop = true;
                  spdlog::info("No more windows - closing event loop");
            }
#endif
      }
}

auto PlatformWin32::open_window(int x, int y, int width, int height, const std::string title) -> std::shared_ptr<PlatformWindow>
{
      auto window = std::make_shared<PlatformWindowWin32>();
      window->title = title;

      HINSTANCE hInstance = GetModuleHandle(NULL);
      WNDCLASSEX wc = {};
      wc.cbSize = sizeof(WNDCLASSEX);
      wc.style = CS_VREDRAW | CS_HREDRAW;
      wc.lpfnWndProc = svision_wndproc;
      wc.hInstance = hInstance;
      wc.lpszClassName = "svision2";
      RegisterClassEx(&wc);
      window->hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
                                    "svision2",
                                    window->title.c_str(),
                                    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                    width, height,
                                    NULL, NULL,
                                    hInstance, NULL);

      if (window->hwnd == NULL)
      {
            return nullptr;
      }

      SetWindowLongPtr(window->hwnd, GWLP_USERDATA, (LONG_PTR)window.get());
      return window;
}

auto PlatformWin32::show_window(std::shared_ptr<PlatformWindow> w) -> void
{
      auto window = std::dynamic_pointer_cast<PlatformWindowWin32>(w);
      ShowWindow(window->hwnd, SW_NORMAL);
      UpdateWindow(window->hwnd);
}
