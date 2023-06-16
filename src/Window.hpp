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
    if (!split & !swapped)
    {
        panel = bounds;
    }
    if (!split & swapped)
    {
        return panel;
    }
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
    // if (menu)
    // {
    //     if (!split & !swapped)
    //     {
    //         panel = {
    //             bounds.left,
    //             bounds.top,
    //             bounds.right,
    //             bounds.bottom - 50,
    //         };
    //     }
    //     if (!split & swapped)
    //     {
    //         panel = {0, 0, 0, 0};
    //     }
    //     if (split & !swapped)
    //     {
    //         panel = {
    //             bounds.left,
    //             bounds.top,
    //             bounds.right / 2,
    //             bounds.bottom - 50,
    //         };
    //     }
    //     if (split & swapped)
    //     {
    //         panel = {
    //             bounds.right / 2,
    //             bounds.top,
    //             bounds.right,
    //             bounds.bottom - 50,
    //         };
    //     }
    // }
    // if (!menu)
    // {
    //     if (!split & !swapped)
    //     {
    //         panel = bounds;
    //     }
    //     if (!split & swapped)
    //     {
    //         panel = {0, 0, 0, 0};
    //     }
    //     if (split & !swapped)
    //     {
    //         panel = {
    //             bounds.left,
    //             bounds.top,
    //             bounds.right / 2,
    //             bounds.bottom,
    //         };
    //     }
    //     if (split & swapped)
    //     {
    //         panel = {
    //             bounds.right / 2,
    //             bounds.top,
    //             bounds.right,
    //             bounds.bottom,
    //         };
    //     }
    // }
    return panel;
}

RECT GetSidePanelBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel = {0, 0, 0, 0};
    GetClientRect(window, &bounds);
    if (!split & !swapped)
    {
        return panel;
    }
    if (!split & swapped)
    {
        panel = bounds;
    }
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
    // if (menu)
    // {
    //     if (!split & !swapped)
    //     {
    //         panel = {0, 0, 0, 0};
    //     }
    //     if (!split & swapped)
    //     {
    //         panel = {
    //             bounds.left,
    //             bounds.top,
    //             bounds.right,
    //             bounds.bottom - 50,
    //         };
    //     }
    //     if (split & !swapped)
    //     {
    //         panel = {
    //             bounds.right / 2,
    //             bounds.top,
    //             bounds.right,
    //             bounds.bottom - 50,
    //         };
    //     }
    //     if (split & swapped)
    //     {
    //         panel = {
    //             bounds.left,
    //             bounds.top,
    //             bounds.right / 2,
    //             bounds.bottom - 50,
    //         };
    //     }
    // }
    // if (!menu)
    // {
    //     if (!split & !swapped)
    //     {
    //         panel = {0, 0, 0, 0};
    //     }
    //     if (!split & swapped)
    //     {
    //         panel = {
    //             bounds.left,
    //             bounds.top,
    //             bounds.right,
    //             bounds.bottom,
    //         };
    //     }
    //     if (split & !swapped)
    //     {
    //         panel = {
    //             bounds.right / 2,
    //             bounds.top,
    //             bounds.right,
    //             bounds.bottom,
    //         };
    //     }
    //     if (split & swapped)
    //     {
    //         panel = {
    //             bounds.left,
    //             bounds.top,
    //             bounds.right / 2,
    //             bounds.bottom,
    //         };
    //     }
    // }
    return panel;
}

bool PanelHideMenu(HWND window)
{
    if (!menu)
    {
        menu = true;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        SetWindowTitle(window);
        SetWindowIcon(window);
        return true;
    }
    else
    {
        menu = false;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        SetWindowTitle(window);
        SetWindowIcon(window);
        return false;
    }
}

bool PanelSplit(HWND window)
{
    if (!split)
    {
        split = true;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        SetWindowTitle(window);
        SetWindowIcon(window);
        return true;
    }
    else
    {
        split = false;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        SetWindowTitle(window);
        SetWindowIcon(window);
        return false;
    }
}

bool PanelSwap(HWND window)
{
    if (!swapped)
    {
        swapped = true;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        SetWindowTitle(window);
        SetWindowIcon(window);
        return true;
    }

    else
    {
        swapped = false;
        SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        SetWindowTitle(window);
        SetWindowIcon(window);
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
            SetWindowTitle(window);
            SetWindowIcon(window);
            return true;
        }
        if (wp.showCmd == SW_SHOWMAXIMIZED)
        {
            ShowWindow(window, SW_SHOWNORMAL);
            SetWindowTitle(window);
            SetWindowIcon(window);
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
            SetWindowTitle(window);
            SetWindowIcon(window);
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
        SetWindowTitle(window);
        SetWindowIcon(window);
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
        SetWindowTitle(window);
        SetWindowIcon(window);
        return false;
    }
    else
    {
        ontop = true;
        SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
        SetWindowTitle(window);
        SetWindowIcon(window);
        return true;
    }
    return false;
}
