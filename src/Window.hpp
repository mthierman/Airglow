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

void SetWindowTitle(HWND window)
{
    std::wstring titleTop = L" [On Top]";
    if (!swapped)
    {
        wil::unique_cotaskmem_string wv_title;
        wv->get_DocumentTitle(&wv_title);
        auto title = wv_title.get();
        if (!ontop)
            SetWindowTextW(window, title);
        if (ontop)
        {
            std::wstring add = title + titleTop;
            SetWindowTextW(window, add.c_str());
        }
    }
    else
    {
        wil::unique_cotaskmem_string wv_title;
        wv2->get_DocumentTitle(&wv_title);
        auto title = wv_title.get();
        if (!ontop)
            SetWindowTextW(window, title);
        if (ontop)
        {
            std::wstring add = title + titleTop;
            SetWindowTextW(window, add.c_str());
        }
    }
}

void SetWindowIcon(HWND window)
{
    using namespace Microsoft::WRL;

    if (!swapped)
    {
        wv->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                       Callback<ICoreWebView2GetFaviconCompletedHandler>(
                           [window](HRESULT result, IStream* iconStream) -> HRESULT
                           {
                               if (iconStream != nullptr)
                               {
                                   Gdiplus::Bitmap iconBitmap(iconStream);
                                   wil::unique_hicon icon;
                                   if (iconBitmap.GetHICON(&icon) == Gdiplus::Status::Ok)
                                   {
                                       auto favicon = std::move(icon);
                                       SendMessageW(window, WM_SETICON, ICON_BIG,
                                                    (LPARAM)favicon.get());
                                   }
                               }
                               return S_OK;
                           })
                           .Get());
    }
    if (swapped)
    {
        wv2->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                        Callback<ICoreWebView2GetFaviconCompletedHandler>(
                            [window](HRESULT result, IStream* iconStream) -> HRESULT
                            {
                                if (iconStream != nullptr)
                                {
                                    Gdiplus::Bitmap iconBitmap(iconStream);
                                    wil::unique_hicon icon;
                                    if (iconBitmap.GetHICON(&icon) == Gdiplus::Status::Ok)
                                    {
                                        auto favicon = std::move(icon);
                                        SendMessageW(window, WM_SETICON, ICON_BIG,
                                                     (LPARAM)favicon.get());
                                    }
                                }
                                return S_OK;
                            })
                            .Get());
    }
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

bool PanelHideMenu(HWND window)
{
    if (!menu)
    {
        menu = true;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        return true;
    }
    else
    {
        menu = false;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        return false;
    }
}

bool PanelSplit(HWND window)
{
    if (!split)
    {
        split = true;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        return true;
    }
    else
    {
        split = false;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        return false;
    }
}

bool PanelSwap(HWND window)
{
    if (!swapped)
    {
        swapped = true;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        return true;
    }

    else
    {
        swapped = false;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        return false;
    }
}

bool WindowMaximize(HWND window)
{
    WINDOWPLACEMENT wp = {};
    wp.length = sizeof(WINDOWPLACEMENT);
    auto placement = GetWindowPlacement(window, &wp);
    if (!fullscreen)
    {
        if (wp.showCmd == SW_SHOWNORMAL)
        {
            ShowWindow(window, SW_MAXIMIZE);
            SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            return true;
        }
        if (wp.showCmd == SW_SHOWMAXIMIZED)
        {
            ShowWindow(window, SW_SHOWNORMAL);
            SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
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
        ontop = false;
        SetWindowPos(window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
        return false;
    }
    else
    {
        ontop = true;
        SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
        return true;
    }
    return false;
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
        {
            return true;
        }
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

std::vector<int> GetBounds(HWND window)
{
    std::vector<int> bounds = {0, 0, 0, 0};
    RECT r;
    if (GetWindowRect(window, &r))
    {
        auto x = r.left;
        auto y = r.top;
        auto cx = r.right - r.left;
        auto cy = r.bottom - r.top;
        bounds = {x, y, cx, cy};
    }
    return bounds;
}

RECT BoundsToRect(HWND window, std::vector<int> bounds)
{
    RECT rect;
    rect.left = bounds[0];
    rect.top = bounds[1];
    rect.right = bounds[0] + bounds[2];
    rect.bottom = bounds[1] + bounds[3];
    return rect;
}

void Startup(HWND window)
{
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    auto settingsFile = GetSettingsFilePath();
    nlohmann::json settings = LoadSettings(settingsFile);
    dimensions = settings["dimensions"].get<std::vector<int>>();
    fullscreen = settings["fullscreen"].get<bool>();
    maximized = settings["maximized"].get<bool>();
    menu = settings["menu"].get<bool>();
    ontop = settings["ontop"].get<bool>();
    split = settings["split"].get<bool>();
    swapped = settings["swapped"].get<bool>();
    mainpage = ToWide(settings["mainpage"].get<std::string>());
    sidepage = ToWide(settings["sidepage"].get<std::string>());
    if (fullscreen)
        WindowFullscreen(window);
    if (maximized)
        WindowMaximize(window);
    if (ontop)
        WindowTop(window);
    SetWindowPos(window, nullptr, dimensions[0], dimensions[1], dimensions[2], dimensions[3], 0);
}

void Shutdown(HWND window)
{
    Gdiplus::GdiplusShutdown(gdiplusToken);
    auto settingsFile = GetSettingsFilePath();
    nlohmann::json settings = CurrentSettings(window);
    SaveSettings(settings, settingsFile);
    DestroyWindow(window);
}
