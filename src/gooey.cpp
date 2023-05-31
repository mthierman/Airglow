#include "gooey.hpp"
#include "resource.hpp"

int WINAPI wWinMain(HINSTANCE histance, HINSTANCE hprevinstance, PWSTR pcmdline, int ncmdshow)
{
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    SetEnvironmentVariable(wvBackgroundColor.c_str(), wvBackgroundColorValue.c_str());

    SetEnvironmentVariable(wvAdditionalBrowserArgs.c_str(), wvAdditionalBrowserArgsValue.c_str());

    icon = (HICON)LoadImage(histance, programIcon.c_str(), IMAGE_ICON, 0, 0,
                            LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);

    cursor = (HCURSOR)LoadImage(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);

    hollowBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

    darkBrush = CreateSolidBrush(RGB(32, 32, 32));

    lightBrush = CreateSolidBrush(RGB(243, 243, 243));

    wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = histance;
    wcex.hCursor = cursor;
    wcex.hbrBackground = hollowBrush;
    wcex.lpszMenuName = menuName.c_str();
    wcex.lpszClassName = className.c_str();
    wcex.hIcon = icon;
    wcex.hIconSm = icon;

    RegisterClassEx(&wcex);

    auto hwnd = CreateWindowEx(0, className.c_str(), windowName.c_str(), WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                               nullptr, histance, nullptr);

    if (!hwnd)
    {
        return 0;
    }

    darkMode = ModeCheck();
    DarkTitle();
    DarkMode(hwnd);
    // ExtendFrame(hwnd);
    ShowWindow(hwnd, ncmdshow);

    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hwnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                env->CreateCoreWebView2Controller(
                    hwnd, Microsoft::WRL::Callback<
                              ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                              [hwnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                              {
                                  if (controller != nullptr)
                                  {
                                      wv_controller = controller;
                                      wv_controller->get_CoreWebView2(&wv);
                                  }

                                  wv->get_Settings(&wv_settings);

                                  wv_settings->put_AreDefaultContextMenusEnabled(true);
                                  wv_settings->put_AreDefaultScriptDialogsEnabled(true);
                                  wv_settings->put_AreDevToolsEnabled(true);
                                  wv_settings->put_AreHostObjectsAllowed(true);
                                  wv_settings->put_IsBuiltInErrorPageEnabled(true);
                                  wv_settings->put_IsScriptEnabled(true);
                                  wv_settings->put_IsStatusBarEnabled(true);
                                  wv_settings->put_IsWebMessageEnabled(true);
                                  wv_settings->put_IsZoomControlEnabled(true);

                                  //   RECT bounds;

                                  //   GetClientRect(hwnd, &bounds);

                                  //   auto webViewWidth = bounds.right - bounds.left;
                                  //   RECT webViewBounds = (bounds.left, bounds.top, bounds.right,
                                  //        bounds.bottom);

                                  //   testRect = {
                                  //       bounds.left,
                                  //       bounds.top,
                                  //       bounds.right / 2,
                                  //       bounds.bottom,
                                  //   };

                                  //   wv_controller->put_Bounds(testRect);

                                  WebViewNavigate(wv);

                                  EventRegistrationToken token;

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
                                          [hwnd](ICoreWebView2* webview,
                                                 ICoreWebView2WebMessageReceivedEventArgs* args)
                                              -> HRESULT
                                          {
                                              wil::unique_cotaskmem_string message;

                                              args->TryGetWebMessageAsString(&message);

                                              if ((std::wstring)message.get() == keyTop.c_str())
                                              {
                                                  KeyTop(hwnd);
                                              }

                                              if ((std::wstring)message.get() == keyMax.c_str())
                                              {
                                                  KeyMaximize(hwnd);
                                              }

                                              if ((std::wstring)message.get() == keyFull.c_str())
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

    auto menu = CreateMenu();

    msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        if (darkMode)
        {
            FillRect(hdc, &rect, darkBrush);
        }
        if (!darkMode)
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
        darkMode = ModeCheck();
        InvalidateRect(hwnd, NULL, true);
        DarkMode(hwnd);
    }
    break;

    case WM_SIZE:
    {
        if (wv_controller != nullptr)
        {
            RECT bounds;
            GetClientRect(hwnd, &bounds);
            testRect = {
                bounds.left,
                bounds.top,
                bounds.right / 2,
                bounds.bottom,
            };
            wv_controller->put_Bounds(testRect);
        }
    }
    break;

    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO minmax = (LPMINMAXINFO)lp;
        minmax->ptMinTrackSize.x = 300;
        minmax->ptMinTrackSize.y = 39;
    }
    break;

    case WM_KEYDOWN:
    {
        if (wp == vkKeyTop)
        {
            KeyTop(hwnd);
        }
        if (wp == vkKeyMax)
        {
            KeyMaximize(hwnd);
        }
        if (wp == vkKeyFull)
        {
            KeyFullscreen(hwnd);
        }
    }
    break;

    case WM_CLOSE:
    {
        DestroyWindow(hwnd);
    }
    break;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;

    default:
        return DefWindowProcW(hwnd, msg, wp, lp);
    }

    return 0;
}

int ModeCheck()
{
    settingsCheck = winrt::Windows::UI::ViewManagement::UISettings();
    fgCheck =
        settingsCheck.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);
    return (((5 * fgCheck.G) + (2 * fgCheck.R) + fgCheck.B) > (8 * 128));
}

void DarkTitle()
{
    using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode appMode);
    uxtheme = LoadLibraryEx(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (uxtheme)
    {
        ord135 = GetProcAddress(uxtheme, PCSTR MAKEINTRESOURCEW(135));
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
    dwmtrue = TRUE;
    dwmfalse = FALSE;
    if (darkMode)
    {
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmtrue, sizeof(dwmtrue));
    }
    if (!darkMode)
    {
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmfalse, sizeof(dwmfalse));
    }
}

void SetMica(HWND hwnd)
{
    hrMica = S_OK;
    mica = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;
    hrMica = DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &mica, sizeof(&mica));
}

void ExtendFrame(HWND hwnd)
{
    m = {-1, -1, -1, -1};
    hrExtend = S_OK;
    hrExtend = DwmExtendFrameIntoClientArea(hwnd, &m);
    if (SUCCEEDED(hrExtend))
    {
        SetMica(hwnd);
    }
}

void KeyTop(HWND hwnd)
{
    topMost = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
    if (topMost & WS_EX_TOPMOST)
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
    wp = {};
    wp.length = sizeof(WINDOWPLACEMENT);
    placement = GetWindowPlacement(hwnd, &wp);
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
    style = GetWindowLongPtr(hwnd, GWL_STYLE);
    if (style & WS_OVERLAPPEDWINDOW)
    {
        mi = {sizeof(mi)};
        GetWindowRect(hwnd, &position);
        if (GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi))
        {
            SetWindowLongPtr(hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
    else
    {
        SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        SetWindowPos(hwnd, nullptr, position.left, position.top, (position.right - position.left),
                     (position.bottom - position.top), 0);
    }
}

void WebViewNavigate(wil::com_ptr<ICoreWebView2> wv)
{
    commandLine = GetCommandLineW();
    commandLineList = CommandLineToArgvW(commandLine, &nArgs);
    if (0 == commandLineList[1])
    {
        wv->Navigate(L"about:blank");
    }
    for (i = 1; i < nArgs; i++)
    {
        wv->Navigate(commandLineList[i]);
    }
    LocalFree(commandLineList);
}
