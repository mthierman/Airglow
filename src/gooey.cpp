#include "gooey.hpp"

#define PROGRAM_ICON 1

int APIENTRY wWinMain(HINSTANCE histance, HINSTANCE hprevinstance, PWSTR pcmdline, int ncmdshow)
{
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    SetEnvironmentVariableW(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR", L"0");
    SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            L"--enable-features=OverlayScrollbar,"
                            L"msOverlayScrollbarWinStyle:scrollbar_mode/"
                            L"full_mode,msOverlayScrollbarWinStyleAnimation,"
                            L"msWebView2BrowserHitTransparent"
                            L" --disable-features=msWebOOUI,msPdfOOUI");

    auto icon = (HICON)LoadImageW(histance, L"PROGRAM_ICON", IMAGE_ICON, 0, 0,
                                  LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);

    darkBrush = CreateSolidBrush(RGB(0, 0, 0));
    lightBrush = CreateSolidBrush(RGB(255, 255, 255));
    hollowBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = histance;
    wcex.hCursor = (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    wcex.hbrBackground = hollowBrush;
    wcex.lpszMenuName = L"menu";
    wcex.lpszClassName = L"window";
    wcex.hIcon = icon;
    wcex.hIconSm = icon;

    RegisterClassExW(&wcex);

    HWND hwnd =
        CreateWindowExW(0, L"window", L"Gooey", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                        CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, histance, nullptr);

    if (!hwnd)
    {
        return 0;
    }

    lightMode = ModeCheck();
    DarkTitle();
    DarkMode(hwnd);
    ShowWindow(hwnd, ncmdshow);

    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hwnd](HRESULT result, ICoreWebView2Environment *env) -> HRESULT
            {
                env->CreateCoreWebView2Controller(
                    hwnd, Microsoft::WRL::Callback<
                              ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                              [hwnd](HRESULT result, ICoreWebView2Controller *controller) -> HRESULT
                              {
                                  if (controller != nullptr)
                                  {
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

                                  wv->ExecuteScript(L"document.onreadystatechange = () => {if "
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
                                      Microsoft::WRL::Callback<
                                          ICoreWebView2WebMessageReceivedEventHandler>(
                                          [hwnd](ICoreWebView2 *webview,
                                                 ICoreWebView2WebMessageReceivedEventArgs *args)
                                              -> HRESULT
                                          {
                                              wil::unique_cotaskmem_string message;
                                              args->TryGetWebMessageAsString(&message);
                                              if ((std::wstring)message.get() == L"F1")
                                              {
                                                  KeyTop(hwnd);
                                              }
                                              if ((std::wstring)message.get() == L"F2")
                                              {
                                                  KeyMaximize(hwnd);
                                              }
                                              if ((std::wstring)message.get() == L"F11")
                                              {
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
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        if (lightMode)
        {
            FillRect(hdc, &rect, darkBrush);
        }
        if (!lightMode)
        {
            FillRect(hdc, &rect, lightBrush);
        }
    }
    break;
    case WM_SETFOCUS:
    {
        if (wv_controller != nullptr)
        {
            wv_controller->MoveFocus(
                COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        }
    }
    break;
    case WM_SETTINGCHANGE:
    {
        lightMode = ModeCheck();
        InvalidateRect(hwnd, NULL, true);
        DarkMode(hwnd);
    }
    break;
    case WM_SIZE:
        if (wv_controller != nullptr)
        {
            RECT bounds;
            GetClientRect(hwnd, &bounds);
            wv_controller->put_Bounds(bounds);
        }
        break;
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lp = (LPMINMAXINFO)lparam;
        lp->ptMinTrackSize.x = 300;
        lp->ptMinTrackSize.y = 39;
    }
    break;
    case WM_KEYDOWN:
        if (wparam == VK_F1)
        {
            KeyTop(hwnd);
        }
        if (wparam == VK_F2)
        {
            KeyMaximize(hwnd);
        }
        if (wparam == VK_F11)
        {
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

int ModeCheck()
{
    auto settings = winrt::Windows::UI::ViewManagement::UISettings();
    auto foreground =
        settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);
    return (((5 * foreground.G) + (2 * foreground.R) + foreground.B) > (8 * 128));
}

void DarkTitle()
{
    enum PreferredAppMode
    {
        Default,
        AllowDark,
        ForceDark,
        ForceLight,
        Max
    };
    using fnSetPreferredAppMode = PreferredAppMode(WINAPI *)(PreferredAppMode appMode);
    auto uxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (uxtheme)
    {
        auto ord135 = GetProcAddress(uxtheme, PCSTR MAKEINTRESOURCEW(135));
        if (ord135)
        {
            auto SetPreferredAppMode = reinterpret_cast<fnSetPreferredAppMode>(ord135);
            SetPreferredAppMode(PreferredAppMode::AllowDark);
        }
        FreeLibrary(uxtheme);
    }
}

void DarkMode(HWND hwnd)
{
    auto dwmtrue = TRUE;
    auto dwmfalse = FALSE;
    auto mode = ModeCheck();
    if (mode)
    {
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmtrue, sizeof(dwmtrue));
    }
    if (!mode)
    {
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmfalse, sizeof(dwmfalse));
    }
}

void SetMica(HWND hwnd)
{
    HRESULT hr = S_OK;
    auto mica = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;
    hr = DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &mica, sizeof(&mica));
}

void ExtendFrame(HWND hwnd)
{
    MARGINS m = {-1, -1, -1, -1};
    HRESULT hr = S_OK;
    hr = DwmExtendFrameIntoClientArea(hwnd, &m);
    if (SUCCEEDED(hr))
    {
        SetMica(hwnd);
    }
}

void KeyTop(HWND hwnd)
{
    auto topmost = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
    if (topmost & WS_EX_TOPMOST)
    {
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
    else
    {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
}

void KeyMaximize(HWND hwnd)
{
    WINDOWPLACEMENT wp = {};
    wp.length = sizeof(WINDOWPLACEMENT);
    auto placement = GetWindowPlacement(hwnd, &wp);
    if (wp.showCmd == SW_SHOWNORMAL)
    {
        ShowWindow(hwnd, SW_MAXIMIZE);
    }
    if (wp.showCmd == SW_SHOWMAXIMIZED)
    {
        ShowWindow(hwnd, SW_SHOWNORMAL);
    }
}

void KeyFullscreen(HWND hwnd)
{
    static RECT position;
    auto style = GetWindowLongPtrW(hwnd, GWL_STYLE);
    if (style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi = {sizeof(mi)};
        GetWindowRect(hwnd, &position);
        if (GetMonitorInfoW(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi))
        {
            SetWindowLongPtrW(hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
    else
    {
        SetWindowLongPtrW(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        SetWindowPos(hwnd, nullptr, position.left, position.top, (position.right - position.left),
                     (position.bottom - position.top), 0);
    }
}

void WebViewNavigate(wil::com_ptr<ICoreWebView2> wv)
{
    auto commandline = GetCommandLineW();
    LPWSTR *szArglist;
    int nArgs;
    int i;
    szArglist = CommandLineToArgvW(commandline, &nArgs);
    if (0 == szArglist[1])
    {
        wv->Navigate(L"about:blank");
    }
    for (i = 1; i < nArgs; i++)
    {
        wv->Navigate(szArglist[i]);
    }
    LocalFree(szArglist);
}
