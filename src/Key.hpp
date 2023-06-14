void KeyTop(HWND window)
{
    auto topMost = GetWindowLongPtrW(window, GWL_EXSTYLE);
    if (topMost & WS_EX_TOPMOST)
    {
        SetWindowPos(window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
    else
    {
        SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
    isTopmost = true;
}

void KeySplit(HWND window)
{
    if (!isSplit)
    {
        isSplit = true;
    }
    else if (isSplit)
    {
        isSplit = false;
    }
    SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void KeyMaximize(HWND window)
{
    WINDOWPLACEMENT wp = {};
    wp.length = sizeof(WINDOWPLACEMENT);
    auto placement = GetWindowPlacement(window, &wp);
    if (!isFullscreen)
    {
        if (wp.showCmd == SW_SHOWNORMAL)
        {
            ShowWindow(window, SW_MAXIMIZE);
            isMaximized = true;
        }
        if (wp.showCmd == SW_SHOWMAXIMIZED)
        {
            ShowWindow(window, SW_SHOWNORMAL);
            isMaximized = false;
        }
    }
}

void KeyFullscreen(HWND window)
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
            isFullscreen = true;
        }
    }
    else
    {
        SetWindowLongPtrW(window, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(window, nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        SetWindowPos(window, nullptr, position.left, position.top, (position.right - position.left),
                     (position.bottom - position.top), 0);
        isFullscreen = false;
    }
}

void KeyClose(HWND window) { SendMessageW(window, WM_CLOSE, 0, 0); }
