void KeyTop(HWND hwnd)
{
    auto topMost = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
    if (topMost & WS_EX_TOPMOST)
    {
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
    else
    {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
    isTopmost = true;
}

void KeySplit(HWND hwnd)
{
    // OutputDebugStringW(L"KEYSPLIT PUSHED");
    // OutputDebugStringW(std::to_wstring(isSplit).c_str());
    if (!isSplit)
    {
        isSplit = true;
    }
    else if (isSplit)
    {
        isSplit = false;
    }
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void KeyMaximize(HWND hwnd)
{
    WINDOWPLACEMENT wp = {};
    wp.length = sizeof(WINDOWPLACEMENT);
    auto placement = GetWindowPlacement(hwnd, &wp);
    if (!isFullscreen)
    {
        if (wp.showCmd == SW_SHOWNORMAL)
        {
            ShowWindow(hwnd, SW_MAXIMIZE);
            isMaximized = true;
        }
        if (wp.showCmd == SW_SHOWMAXIMIZED)
        {
            ShowWindow(hwnd, SW_SHOWNORMAL);
            isMaximized = false;
        }
    }
}

void KeyFullscreen(HWND hwnd)
{
    auto style = GetWindowLongPtrW(hwnd, GWL_STYLE);
    static RECT position;
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
            isFullscreen = true;
        }
    }
    else
    {
        SetWindowLongPtrW(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        SetWindowPos(hwnd, nullptr, position.left, position.top, (position.right - position.left),
                     (position.bottom - position.top), 0);
        isFullscreen = false;
    }
}

void KeyClose(HWND hwnd) { SendMessageW(hwnd, WM_CLOSE, 0, 0); }
