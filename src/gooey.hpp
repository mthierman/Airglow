using namespace Microsoft::WRL;
using namespace winrt::Windows::UI::ViewManagement;

int ModeCheck()
{
    auto settings = UISettings();
    auto foreground = settings.GetColorValue(UIColorType::Foreground);
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
