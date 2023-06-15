unsigned short MakeWindowClass(HINSTANCE instance)
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = instance;
    wcex.hCursor = (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = menuName.c_str();
    wcex.lpszClassName = className.c_str();
    wcex.hIcon = (HICON)LoadImageW(instance, programIcon.c_str(), IMAGE_ICON, 0, 0,
                                   LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
    wcex.hIconSm = (HICON)LoadImageW(instance, programIcon.c_str(), IMAGE_ICON, 0, 0,
                                     LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
    return RegisterClassExW(&wcex);
}

HWND MakeWindow(HINSTANCE instance)
{
    return CreateWindowExW(0, className.c_str(), windowName.c_str(), WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                           nullptr, instance, nullptr);
}

HWND InitializeWindow(HINSTANCE instance, int ncs)
{
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    SetEnvironmentVariableW(wvBackgroundColor.c_str(), wvBackgroundColorValue.c_str());
    SetEnvironmentVariableW(wvAdditionalBrowserArgs.c_str(), wvAdditionalBrowserArgsValue.c_str());
    auto args = CommandLineUrl();
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    auto atom = MakeWindowClass(instance);
    if (!atom)
    {
        return 0;
    }
    auto window = MakeWindow(instance);
    if (!window)
    {
        return 0;
    }
    SetDarkTitle();
    SetDarkMode(window);
    SetMica(window);
    SetWindow(window, ncs);
    return window;
}

RECT GetWindowBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    GetClientRect(window, &bounds);
    return bounds;
}

RECT GetHiddenPanelBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    return bounds;
}

RECT GetFullPanelBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel;
    GetClientRect(window, &bounds);
    if (panelMenu)
    {
        panel = {
            bounds.left,
            bounds.top,
            bounds.right,
            bounds.bottom - 50,
        };
    }
    if (!panelMenu)
    {
        panel = {
            bounds.left,
            bounds.top,
            bounds.right,
            bounds.bottom,
        };
    }
    return panel;
}

RECT GetLeftPanelBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel;
    GetClientRect(window, &bounds);
    if (panelMenu)
    {
        panel = {
            bounds.left,
            bounds.top,
            bounds.right / 2,
            bounds.bottom - 50,
        };
    }
    if (!panelMenu)
    {
        panel = {
            bounds.left,
            bounds.top,
            bounds.right / 2,
            bounds.bottom,
        };
    }
    return panel;
}

RECT GetRightPanelBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel;
    GetClientRect(window, &bounds);
    if (panelMenu)
    {
        panel = {
            bounds.right / 2,
            bounds.top,
            bounds.right,
            bounds.bottom - 50,
        };
    }
    if (!panelMenu)
    {
        panel = {
            bounds.right / 2,
            bounds.top,
            bounds.right,
            bounds.bottom,
        };
    }
    return panel;
}

RECT GetBottomPanelBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel;
    GetClientRect(window, &bounds);
    if (panelMenu)
    {
        panel = {
            bounds.left,
            bounds.bottom - 75,
            bounds.right,
            bounds.bottom,
        };
    }
    if (!panelMenu)
    {
        panel = {0, 0, 0, 0};
    }
    return panel;
}

bool WindowTop(HWND window)
{
    FLASHWINFO fwi;
    fwi.cbSize = sizeof(FLASHWINFO);
    fwi.hwnd = window;
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 0;
    auto topMost = GetWindowLongPtrW(window, GWL_EXSTYLE);
    if (topMost & WS_EX_TOPMOST)
    {
        SetWindowPos(window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
        auto documentTitle = title.get();
        std::wstring changeTitle = std::wstring(documentTitle);
        SetWindowTextW(window, changeTitle.c_str());
        return false;
    }
    else
    {
        SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
        auto documentTitle = title.get();
        std::wstring changeTitle = std::wstring(documentTitle) + L" [On Top]";
        SetWindowTextW(window, changeTitle.c_str());
        return true;
    }
    return false;
}

bool WindowMaximize(HWND window)
{
    WINDOWPLACEMENT wp = {};
    wp.length = sizeof(WINDOWPLACEMENT);
    auto placement = GetWindowPlacement(window, &wp);
    if (!isFullscreen)
    {
        if (wp.showCmd == SW_SHOWNORMAL)
        {
            ShowWindow(window, SW_MAXIMIZE);
            return true;
        }
        if (wp.showCmd == SW_SHOWMAXIMIZED)
        {
            ShowWindow(window, SW_SHOWNORMAL);
            return false;
        }
    }
    return false;
}

bool WindowFullscreen(HWND window)
{
    auto style = GetWindowLongPtrW(window, GWL_STYLE);
    static RECT position;
    if (style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi = {sizeof(mi)};
        GetWindowRect(window, &position);
        if (GetMonitorInfoW(MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST), &mi))
        {
            SetWindowLongPtrW(window, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
            return true;
        }
    }
    else
    {
        SetWindowLongPtrW(window, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(window, nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        SetWindowPos(window, nullptr, position.left, position.top, (position.right - position.left),
                     (position.bottom - position.top), 0);
        return false;
    }
    return false;
}

bool PanelSplit(HWND window)
{
    if (!isSplit)
    {
        isSplit = true;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        return true;
    }
    else
    {
        isSplit = false;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        return false;
    }
}

bool PanelHideMenu(HWND window)
{
    if (!panelMenu)
    {
        panelMenu = true;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        return true;
    }
    else
    {
        panelMenu = false;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        return false;
    }
}
