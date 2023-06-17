void Messages(std::wstring message)
{
    std::wstring splitKey = std::wstring(L"F1");
    std::wstring swapKey = std::wstring(L"F2");
    std::wstring hideMenuKey = std::wstring(L"F4");
    std::wstring maximizeKey = std::wstring(L"F5");
    std::wstring fullscreenKey = std::wstring(L"F11");
    std::wstring onTopKey = std::wstring(L"F9");
    std::wstring closeKey = std::wstring(L"close");

    if (message == splitKey)
    {
        split = SplitPanel();
        SendMessageW(window, WM_SIZE, 0, 0);
        SendMessageW(window, WM_SETFOCUS, 0, 0);
    }

    if (message == swapKey)
    {
        swapped = SwapPanel();
        SendMessageW(window, WM_SIZE, 0, 0);
        SendMessageW(window, WM_SETFOCUS, 0, 0);
    }

    if (message == hideMenuKey)
    {
        menu = HideMenu();
        SendMessageW(window, WM_SIZE, 0, 0);
        SendMessageW(window, WM_SETFOCUS, 0, 0);
    }

    if (message == maximizeKey)
    {
        maximized = WindowMaximize(window);
        SendMessageW(window, WM_SIZE, 0, 0);
        SendMessageW(window, WM_SETFOCUS, 0, 0);
    }

    if (message == fullscreenKey)
    {
        fullscreen = WindowFullscreen(window);
        SendMessageW(window, WM_SIZE, 0, 0);
        SendMessageW(window, WM_SETFOCUS, 0, 0);
    }

    if (message == onTopKey)
    {
        ontop = WindowTop(window);
        SendMessageW(window, WM_SIZE, 0, 0);
        SendMessageW(window, WM_SETFOCUS, 0, 0);
    }

    if (message == closeKey)
    {
        SendMessageW(window, WM_CLOSE, 0, 0);
    }
}
