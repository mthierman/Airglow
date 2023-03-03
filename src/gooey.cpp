#include "gooey.h"

using namespace Microsoft::WRL;
using namespace winrt::Windows::UI::ViewManagement;

enum PreferredAppMode { Default, AllowDark, ForceDark, ForceLight, Max };

using fnSetPreferredAppMode =
    PreferredAppMode(WINAPI *)(PreferredAppMode appMode);

static RECT position;

static wil::com_ptr<ICoreWebView2Controller> wv_controller;
static wil::com_ptr<ICoreWebView2> wv;
static wil::com_ptr<ICoreWebView2Settings> wv_settings;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void Navigate() {
  auto commandline = GetCommandLineW();
  LPWSTR *szArglist;
  int nArgs;
  int i;
  szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
  if (0 == szArglist[1]) {
    wv->Navigate(L"about:blank");
    // webview->Navigate(L"https://google.com/");
  }
  for (i = 1; i < nArgs; i++) {
    wv->Navigate(szArglist[i]);
  }
  LocalFree(szArglist);
}

void SetMica(HWND hwnd) {
  MARGINS m = {-1};
  DwmExtendFrameIntoClientArea(hwnd, &m);
  auto mica = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;
  DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &mica, sizeof(&mica));
}

void SetDarkMode(HWND hWnd) {
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

void SetDarkModeTitle() {
  auto uxtheme =
      LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
  if (uxtheme) {
    auto ord135 = GetProcAddress(uxtheme, PCSTR MAKEINTRESOURCEW(135));
    if (ord135) {
      auto SetPreferredAppMode =
          reinterpret_cast<fnSetPreferredAppMode>(ord135);
      SetPreferredAppMode(PreferredAppMode::AllowDark);
    }
    FreeLibrary(uxtheme);
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

int APIENTRY wWinMain(HINSTANCE histance, HINSTANCE hprevinstance,
                      PWSTR pcmdline, int ncmdshow) {
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
  wcex.hInstance = histance;
  wcex.hCursor = (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR,
                                     0, 0, LR_SHARED);
  wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wcex.lpszMenuName = L"menu";
  wcex.lpszClassName = L"window";
  wcex.hIcon = (HICON)LoadImageW(histance, L"PROGRAM_ICON", IMAGE_ICON, 0, 0,
                                 LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
  wcex.hIconSm =
      (HICON)LoadImageW(histance, L"PROGRAM_ICON", IMAGE_ICON, 0, 0,
                        LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);

  RegisterClassExW(&wcex);

  HWND hwnd = CreateWindowExW(
      0, L"window", L"Gooey", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, histance, nullptr);

  SetDarkModeTitle();

  if (!hwnd) {
    return 0;
  }

  SetMica(hwnd);

  SetDarkMode(hwnd);

  ShowWindow(hwnd, ncmdshow);

  CreateCoreWebView2EnvironmentWithOptions(
      nullptr, nullptr, nullptr,
      Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
          [hwnd](HRESULT result, ICoreWebView2Environment *env) -> HRESULT {
            env->CreateCoreWebView2Controller(
                hwnd,
                Callback<
                    ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [hwnd](HRESULT result,
                           ICoreWebView2Controller *controller) -> HRESULT {
                      if (controller != nullptr) {
                        wv_controller = controller;
                        wv_controller->get_CoreWebView2(&wv);
                      }
                      wv->get_Settings(&wv_settings);
                      wv_settings->put_IsScriptEnabled(TRUE);
                      wv_settings->put_AreDefaultScriptDialogsEnabled(TRUE);
                      wv_settings->put_IsWebMessageEnabled(TRUE);

                      RECT bounds;
                      GetClientRect(hwnd, &bounds);
                      wv_controller->put_Bounds(bounds);

                      Navigate();

                      EventRegistrationToken token;

                      wv->ExecuteScript(
                          L"document.onreadystatechange = () => {if "
                          L"(document.readyState === 'complete') "
                          L"{onkeydown = (e) => "
                          L"{window.chrome.webview.postMessage(e.key);}}}"
                          L";",
                          nullptr);

                      wv->AddScriptToExecuteOnDocumentCreated(
                          L"document.onreadystatechange = () => {if "
                          L"(document.readyState === 'complete') "
                          L"{onkeydown = (e) => "
                          L"{window.chrome.webview.postMessage(e.key);}}}"
                          L";",
                          nullptr);

                      wv->add_WebMessageReceived(
                          Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                              [hwnd](ICoreWebView2 *webview,
                                     ICoreWebView2WebMessageReceivedEventArgs
                                         *args) -> HRESULT {
                                wil::unique_cotaskmem_string message;
                                args->TryGetWebMessageAsString(&message);
                                if ((std::wstring)message.get() == L"F1") {
                                  OnTop(hwnd);
                                }
                                if ((std::wstring)message.get() == L"F11") {
                                  FullScreen(hwnd);
                                }
                                webview->PostWebMessageAsString(message.get());
                                return S_OK;
                              })
                              .Get(),
                          &token);
                      return S_OK;
                    })
                    .Get());
            return S_OK;
          })
          .Get());

  MSG msg = {};
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
  switch (umsg) {
  case WM_SETTINGCHANGE: {
    SetDarkMode(hwnd);
  } break;
  case WM_SIZE:
    if (wv_controller != nullptr) {
      RECT b;
      GetClientRect(hwnd, &b);
      wv_controller->put_Bounds(b);
    }
    break;
  case WM_GETMINMAXINFO: {
    LPMINMAXINFO lp = (LPMINMAXINFO)lparam;
    lp->ptMinTrackSize.x = 600;
    lp->ptMinTrackSize.y = 600;
  } break;
  case WM_KEYDOWN:
    if (wparam == VK_F1) {
      OnTop(hwnd);
    }
    if (wparam == VK_F11) {
      FullScreen(hwnd);
    }
    break;
  case WM_CLOSE:
    DestroyWindow(hwnd);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProcW(hwnd, umsg, wparam, lparam);
  }

  return 0;
}
