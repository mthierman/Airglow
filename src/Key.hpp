bool KeyTop(HWND window)
{
    auto topMost = GetWindowLongPtrW(window, GWL_EXSTYLE);
    FLASHWINFO fwi;
    fwi.cbSize = sizeof(FLASHWINFO);
    fwi.hwnd = window;
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 0;
    if (topMost & WS_EX_TOPMOST)
    {
        SetWindowPos(window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
        auto title = url1 + L" | " + url2;
        SetWindowTextW(window, title.c_str());
        return false;
    }
    else
    {
        SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
        auto title = url1 + L" | " + url2 + L" [On Top]";
        SetWindowTextW(window, title.c_str());
        return true;
    }
    return false;
}

bool KeySplit(HWND window)
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
    return false;
}

bool KeyMaximize(HWND window)
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

bool KeyFullscreen(HWND window)
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

bool KeyClose(HWND window)
{
    SendMessageW(window, WM_CLOSE, 0, 0);
    return true;
}
