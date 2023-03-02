#include "gooey.h"
#include "winrt/impl/Microsoft.Web.WebView2.Core.2.h"
#include <algorithm>
#include <charconv>
#include <format>
#include <iostream>
#include <memory>
#include <string>

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Microsoft::Web::WebView2::Core;

enum PreferredAppMode { Default, AllowDark, ForceDark, ForceLight, Max };
using fnSetPreferredAppMode =
    PreferredAppMode(WINAPI *)(PreferredAppMode appMode);
RECT position;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

std::shared_ptr<CoreWebView2> webview_ptr;
std::shared_ptr<CoreWebView2Controller> controller_ptr;
std::shared_ptr<CoreWebView2Settings> settings_ptr;

IAsyncAction InitWebView2(HWND hWnd) {
  CoreWebView2Environment env = co_await CoreWebView2Environment::CreateAsync();

  auto handle = CoreWebView2ControllerWindowReference::CreateFromWindowHandle(
      reinterpret_cast<std::uint64_t>(hWnd));
  CoreWebView2Controller controller =
      co_await env.CreateCoreWebView2ControllerAsync(handle);
  controller_ptr = std::make_shared<CoreWebView2Controller>(controller);

  auto webview = controller.CoreWebView2();
  webview_ptr = std::make_shared<CoreWebView2>(webview);

  auto settings = webview.Settings();
  settings.IsScriptEnabled(true);
  settings.AreDefaultScriptDialogsEnabled(true);
  settings.IsWebMessageEnabled(true);
  settings_ptr = std::make_shared<CoreWebView2Settings>(settings);

  // if (controller) {
  //   OutputDebugStringW(L"CoreWebView2Controller initialized!\n");
  // }
  // if (webview) {
  //   OutputDebugStringW(L"CoreWebView2 initialized!\n");
  // }
  // if (settings) {
  //   OutputDebugStringW(L"CoreWebView2 settings initialized!\n");
  // }

  // controller.IsVisible(true);

  RECT bounds;
  GetClientRect(hWnd, &bounds);
  float x = static_cast<float>(bounds.left);
  float y = static_cast<float>(bounds.right);
  float width = static_cast<float>(bounds.right - bounds.left);
  float height = static_cast<float>(bounds.bottom - bounds.top);
  Rect newbounds = Rect(x, y, width, height);
  controller.BoundsMode(CoreWebView2BoundsMode::UseRawPixels);
  controller.Bounds(newbounds);
  // controller_ptr->Bounds(newbounds);

  // webview.Navigate(L"https://www.google.com/");
}

void DarkMode(HWND hWnd) {
  auto dwmtrue = TRUE;
  auto dwmfalse = FALSE;
  auto settings = UISettings();
  auto foreground = settings.GetColorValue(UIColorType::Foreground);
  auto modecheck =
      (((5 * foreground.G) + (2 * foreground.R) + foreground.B) > (8 * 128));
  if (modecheck) {
    DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmtrue,
                          sizeof(dwmtrue));
  }
  if (!modecheck) {
    DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmfalse,
                          sizeof(dwmfalse));
  }
}

void OnTop(HWND hWnd) {
  auto topmost = GetWindowLongPtrW(hWnd, GWL_EXSTYLE);
  if (topmost & WS_EX_TOPMOST) {
    SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  } else {
    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  }
}

void FullScreen(HWND hWnd) {
  auto style = GetWindowLongPtrW(hWnd, GWL_STYLE);
  if (style & WS_OVERLAPPEDWINDOW) {
    MONITORINFO mi = {sizeof(mi)};
    GetWindowRect(hWnd, &position);
    if (GetMonitorInfoW(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST),
                        &mi)) {
      SetWindowLongPtrW(hWnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
      SetWindowPos(hWnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                   mi.rcMonitor.right - mi.rcMonitor.left,
                   mi.rcMonitor.bottom - mi.rcMonitor.top,
                   SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
  } else {
    SetWindowLongPtrW(hWnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
    SetWindowPos(hWnd, nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                     SWP_FRAMECHANGED);
    SetWindowPos(hWnd, nullptr, position.left, position.top,
                 (position.right - position.left),
                 (position.bottom - position.top), 0);
  }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine,
                      _In_ int nCmdShow) {
  // winrt::init_apartment(winrt::apartment_type::multi_threaded);
  SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
  SetEnvironmentVariableW(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR", L"0");
  SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                          L"--enable-features=OverlayScrollbar,"
                          L"msOverlayScrollbarWinStyle:scrollbar_mode/"
                          L"full_mode,msOverlayScrollbarWinStyleAnimation "
                          L"--disable-features=msWebOOUI,msPdfOOUI");

  WNDCLASSEXW wcex = {};

  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = 0;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hCursor = (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR,
                                     0, 0, LR_SHARED);
  wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wcex.lpszMenuName = L"menu";
  wcex.lpszClassName = L"window";
  wcex.hIcon = (HICON)LoadImageW(hInstance, L"PROGRAM_ICON", IMAGE_ICON, 0, 0,
                                 LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
  wcex.hIconSm =
      (HICON)LoadImageW(hInstance, L"PROGRAM_ICON", IMAGE_ICON, 0, 0,
                        LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);

  RegisterClassExW(&wcex);

  HWND hWnd = CreateWindowExW(
      0, L"window", L"Gooey", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

  auto hUxtheme =
      LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
  if (hUxtheme) {
    auto ord135 =
        GetProcAddress(_In_ hUxtheme, _In_ PCSTR MAKEINTRESOURCEW(135));
    if (ord135) {
      auto SetPreferredAppMode =
          reinterpret_cast<fnSetPreferredAppMode>(ord135);
      SetPreferredAppMode(PreferredAppMode::AllowDark);
    }
    FreeLibrary(hUxtheme);
  }

  if (!hWnd) {
    return 0;
  }

  MARGINS m = {-1};
  DwmExtendFrameIntoClientArea(hWnd, &m);

  auto backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;
  DwmSetWindowAttribute(hWnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop,
                        sizeof(&backdrop));

  DarkMode(hWnd);

  ShowWindow(hWnd, nCmdShow);

  InitWebView2(hWnd);

  MSG msg = {};
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_SIZE: {
    if (controller_ptr) {
      OutputDebugStringW(L"CoreWebView2Controller initialized!\n");
    }
    if (webview_ptr) {
      OutputDebugStringW(L"CoreWebView2 initialized!\n");
    }
    if (settings_ptr) {
      OutputDebugStringW(L"CoreWebView2Settings initialized!\n");
    }
  } break;
  case WM_SETTINGCHANGE: {
    DarkMode(hWnd);
  } break;
  case WM_GETMINMAXINFO: {
    LPMINMAXINFO lp = (LPMINMAXINFO)lParam;
    lp->ptMinTrackSize.x = 600;
    lp->ptMinTrackSize.y = 600;
  } break;
  case WM_KEYDOWN:
    if (wParam == VK_F1) {
      OnTop(hWnd);
    }
    if (wParam == VK_F11) {
      FullScreen(hWnd);
    }
    break;
  case WM_CLOSE:
    DestroyWindow(hWnd);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
  }

  return 0;
}
