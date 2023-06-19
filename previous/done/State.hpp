bool Toggle(bool b) { return b ? false : true; }

void MaximizeWindow(HWND window)
{
    if (!fullscreen)
    {
        if (!maximized)
            ShowWindow(window, SW_NORMAL);
        else
            ShowWindow(window, SW_MAXIMIZE);
    }
}

void FullscreenWindow(HWND window)
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
        }
    }

    else
    {
        SetWindowLongPtrW(window, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(window, nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        SetWindowPos(window, nullptr, position.left, position.top, (position.right - position.left),
                     (position.bottom - position.top), 0);
    }
}

void TopmostWindow(HWND window)
{
    FLASHWINFO fwi;
    fwi.cbSize = sizeof(FLASHWINFO);
    fwi.hwnd = window;
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 100;

    auto top = GetWindowLongPtrW(window, GWL_EXSTYLE);

    if (top & WS_EX_TOPMOST)
    {
        SetWindowPos(window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
    }

    else
    {
        SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
    }
}

void UpdateFocus()
{
    if (menu)
        if (settings_controller != nullptr)
            settings_controller->MoveFocus(
                COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    if (!swapped & !menu)
        if (main_controller != nullptr)
            main_controller->MoveFocus(
                COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    if (swapped & !menu)
        if (side_controller != nullptr)
            side_controller->MoveFocus(
                COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
}

void UpdateBounds(HWND window)
{
    if (main_controller != nullptr)
        main_controller->put_Bounds(GetMainPanelBounds(window));

    if (side_controller != nullptr)
        side_controller->put_Bounds(GetSidePanelBounds(window));

    if (settings_controller != nullptr)
        settings_controller->put_Bounds(GetMenuBounds(window));
}
