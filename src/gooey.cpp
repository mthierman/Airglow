#include "gooey.hpp"
#include <wingdi.h>
#include <winuser.h>

using namespace Gooey;

static wil::com_ptr<ICoreWebView2Controller> wv_controller;
static wil::com_ptr<ICoreWebView2> wv;
static wil::com_ptr<ICoreWebView2Settings> wv_settings;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(HINSTANCE histance, HINSTANCE hprevinstance,
                      PWSTR pcmdline, int ncmdshow) {

  SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
  SetEnvironmentVariableW(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR", L"0");
  SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                          L"--enable-features=OverlayScrollbar,"
                          L"msOverlayScrollbarWinStyle:scrollbar_mode/"
                          L"full_mode,msOverlayScrollbarWinStyleAnimation "
                          L"--disable-features=msWebOOUI,msPdfOOUI");

  auto icon = (HICON)LoadImageW(histance, L"PROGRAM_ICON", IMAGE_ICON, 0, 0,
                                LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);

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
  wcex.hIcon = icon;
  wcex.hIconSm = icon;

  RegisterClassExW(&wcex);

  HWND hwnd = CreateWindowExW(
      0, L"window", L"Gooey", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, histance, nullptr);

  if (!hwnd) {
    return 0;
  }

  SetDarkMode(hwnd);
  SetDarkModeTitle();
  ExtendFrame(hwnd);
  SetMica(hwnd);
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

                      WebViewNavigate(wv);

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
                                  KeyTop(hwnd);
                                }
                                if ((std::wstring)message.get() == L"F11") {
                                  KeyFullscreen(hwnd);
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
  // case WM_PAINT: {
  //   PAINTSTRUCT ps;
  //   HDC hdc = BeginPaint(hwnd, &ps);
  //   FillRect(hdc, &ps.rcPaint, (HBRUSH)GetStockObject(HOLLOW_BRUSH));
  //   EndPaint(hwnd, &ps);
  // } break;
  // case WM_ERASEBKGND: {
  //   PAINTSTRUCT ps;
  //   HDC hdc = BeginPaint(hwnd, &ps);
  //   FillRect(hdc, &ps.rcPaint, (HBRUSH)GetStockObject(HOLLOW_BRUSH));
  //   EndPaint(hwnd, &ps);
  // } break;
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
    lp->ptMinTrackSize.x = 250;
    lp->ptMinTrackSize.y = 0;
  } break;
  case WM_KEYDOWN:
    if (wparam == VK_F1) {
      KeyTop(hwnd);
    }
    if (wparam == VK_F11) {
      KeyFullscreen(hwnd);
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
