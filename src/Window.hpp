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
    SetEnvironmentVariableW(std::wstring(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR").c_str(),
                            std::wstring(L"0").c_str());

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

std::vector<int> RectToBounds(RECT rect)
{
    std::vector<int> bounds = {rect.left, rect.top, (rect.right - rect.left),
                               (rect.bottom - rect.top)};

    return bounds;
}

RECT BoundsToRect(std::vector<int> bounds)
{
    RECT rect = {bounds[0], bounds[1], (bounds[0] + bounds[2]), (bounds[1] + bounds[3])};

    return rect;
}

RECT GetMenuBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel = {0, 0, 0, 0};

    GetClientRect(window, &bounds);

    if (menu)
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

RECT GetMainPanelBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel = {0, 0, 0, 0};

    GetClientRect(window, &bounds);

    if (menu)
        return panel;

    if (!split & !swapped)
        panel = bounds;

    if (!split & swapped)
        return panel;

    if (split & !swapped)
    {
        panel = {
            bounds.left,
            bounds.top,
            bounds.right / 2,
            bounds.bottom,
        };
    }

    if (split & swapped)
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

RECT GetSidePanelBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel = {0, 0, 0, 0};

    GetClientRect(window, &bounds);

    if (!split & !swapped)
        return panel;

    if (!split & swapped)
        panel = bounds;

    if (split & !swapped)
    {
        panel = {
            bounds.right / 2,
            bounds.top,
            bounds.right,
            bounds.bottom,
        };
    }

    if (split & swapped)
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

bool HideMenu()
{
    if (!menu)
        return true;

    else
        return false;
}

bool SplitPanel()
{
    if (!split)
        return true;

    else
        return false;
}

bool SwapPanel()
{
    if (!swapped)
        return true;

    else
        return false;
}

bool WindowMaximize(HWND window)
{
    if (!fullscreen)
    {
        if (!maximized)
        {
            ShowWindow(window, SW_MAXIMIZE);
            return true;
        }

        else
        {
            ShowWindow(window, SW_SHOWNORMAL);
            return false;
        }
    }

    return false;
}

bool WindowFullscreen(HWND window)
{
    static RECT position;
    auto style = GetWindowLongPtrW(window, GWL_STYLE);

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

bool WindowTop(HWND window)
{
    FLASHWINFO fwi;
    fwi.cbSize = sizeof(FLASHWINFO);
    fwi.hwnd = window;
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 100;

    auto topMost = GetWindowLongPtrW(window, GWL_EXSTYLE);

    if (topMost & WS_EX_TOPMOST)
    {
        SetWindowPos(window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);

        return true;
    }

    else
    {
        SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);

        return false;
    }
}

bool CheckSystemDarkMode()
{
    using namespace winrt::Windows::UI;
    using namespace winrt::Windows::UI::ViewManagement;

    UISettings settingsCheck = UISettings();
    Color fgCheck = settingsCheck.GetColorValue(UIColorType::Foreground);

    return (((5 * fgCheck.G) + (2 * fgCheck.R) + fgCheck.B) > (8 * 128));
}

bool SetDarkTitle()
{
    using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode appMode);

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

        return true;
    }

    return false;
}

bool SetDarkMode(HWND window)
{
    auto dark = CheckSystemDarkMode();
    auto dwmtrue = TRUE;
    auto dwmfalse = FALSE;

    if (dark)
    {
        DwmSetWindowAttribute(window, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmtrue, sizeof(dwmtrue));

        return true;
    }

    if (!dark)
    {
        DwmSetWindowAttribute(window, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmfalse, sizeof(dwmfalse));

        return false;
    }

    return false;
}

bool SetMica(HWND window)
{
    MARGINS m = {0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};
    auto extend = S_OK;

    extend = DwmExtendFrameIntoClientArea(window, &m);

    if (SUCCEEDED(extend))
    {
        auto backdrop = S_OK;
        backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;

        backdrop =
            DwmSetWindowAttribute(window, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(&backdrop));

        if (SUCCEEDED(backdrop))
            return true;

        return false;
    }

    return false;
}

bool SetWindow(HWND window, int ncs)
{
    auto cloakOn = TRUE;
    auto cloakOff = FALSE;
    auto cloak = S_OK;

    cloak = DwmSetWindowAttribute(window, DWMWA_CLOAK, &cloakOn, sizeof(cloakOn));

    if (SUCCEEDED(cloak))
    {
        auto uncloak = S_OK;

        SetWindowPos(window, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
        uncloak = DwmSetWindowAttribute(window, DWMWA_CLOAK, &cloakOff, sizeof(cloakOff));

        if (SUCCEEDED(uncloak))
        {
            ShowWindow(window, ncs);

            return true;
        }

        return false;
    }

    return false;
}

void UpdateFocus()
{
    if (!swapped)
        if (main_controller != nullptr)
            main_controller->MoveFocus(
                COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
    if (swapped)
        if (side_controller != nullptr)
            side_controller->MoveFocus(
                COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
}

void UpdateBounds(HWND window)
{
    // DebugMessages(window);

    if (main_controller != nullptr)
        main_controller->put_Bounds(GetMainPanelBounds(window));

    if (side_controller != nullptr)
        side_controller->put_Bounds(GetSidePanelBounds(window));

    if (settings_controller != nullptr)
        settings_controller->put_Bounds(GetMenuBounds(window));
}

void DebugMessages(HWND window)
{
    std::wstring dpi = L"DPI: " + std::to_wstring(GetDpiForWindow(window)) + L"\n";
    OutputDebugStringW(dpi.c_str());

    RECT client;
    GetClientRect(window, &client);
    std::wstring clientRect = L"ClientRect: " + std::to_wstring(client.right - client.left) +
                              L" x " + std::to_wstring(client.bottom - client.top) + L"\n";
    OutputDebugStringW(clientRect.c_str());

    RECT bounds;
    GetWindowRect(window, &bounds);
    std::wstring windowRect = L"WindowRect: " + std::to_wstring(bounds.right - bounds.left) +
                              L" x " + std::to_wstring(bounds.bottom - bounds.top) + L"\n";
    OutputDebugStringW(windowRect.c_str());
}
