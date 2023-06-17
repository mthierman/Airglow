__int64 __stdcall WndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(window, &ps);
        RECT bounds;
        GetClientRect(window, &bounds);
        FillRect(hdc, &bounds, (HBRUSH)GetStockObject(BLACK_BRUSH));
        EndPaint(window, &ps);
    }
    break;
    case WM_SETTINGCHANGE:
    {
        InvalidateRect(window, nullptr, true);
        SetDarkMode(window);
    }
    break;
    case WM_ACTIVATE:
    {
        // SendMessageW(window, WM_SETFOCUS, 0, 0);
    }
    break;
    case WM_SETFOCUS:
    {
        // SetWindowFocus(window);
    }
    break;
    case WM_WINDOWPOSCHANGING:
    {
        if (!maximized)
        {
            RECT rect;
            GetWindowRect(window, &rect);
            dimensions = RectToBounds(rect);
        }
    }
    break;
    case WM_SIZE:
    {
        WindowResizing(window);
    }
    break;
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO minmax = (LPMINMAXINFO)lparam;
        minmax->ptMinTrackSize.x = 800;
        minmax->ptMinTrackSize.y = 600;
    }
    break;
    case WM_KEYDOWN:
    {
        if (wparam == VK_F1)
        {
            split = SplitPanel();
            SendMessageW(window, WM_SIZE, 0, 0);
        }

        if (wparam == VK_F2)
        {
            swapped = SwapPanel();
            SendMessageW(window, WM_SIZE, 0, 0);
            SetWindowTitle(window);
            SetWindowIcon(window);
        }

        if (wparam == VK_F4)
        {
            menu = HideMenu();
            SendMessageW(window, WM_SIZE, 0, 0);
            SetWindowTitle(window);
            SetWindowIcon(window);
        }

        if (wparam == VK_F5)
        {
            maximized = WindowMaximize(window);
        }

        if (wparam == VK_F11)
        {
            fullscreen = WindowFullscreen(window);
        }

        if (wparam == VK_F9)
        {
            ontop = WindowTop(window);
            SetWindowTitle(window);
            SetWindowIcon(window);
        }

        if (wparam == 0x57)
        {
            auto state = GetKeyState(VK_CONTROL);
            if (state & 0x8000)
                SendMessageW(window, WM_CLOSE, 0, 0);
        }
    }
    break;
    case WM_CLOSE:
    {
        Shutdown(window);
    }
    break;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProcW(window, msg, wparam, lparam);
    }
    return 0;
}

void WebViewMessages(HWND window, std::wstring message)
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
    }

    if (message == swapKey)
    {
        swapped = SwapPanel();
        SendMessageW(window, WM_SIZE, 0, 0);
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if (message == hideMenuKey)
    {
        menu = HideMenu();
        SendMessageW(window, WM_SIZE, 0, 0);
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if (message == maximizeKey)
    {
        maximized = WindowMaximize(window);
    }

    if (message == fullscreenKey)
    {
        fullscreen = WindowFullscreen(window);
    }

    if (message == onTopKey)
    {
        ontop = WindowTop(window);
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if (message == closeKey)
    {
        SendMessageW(window, WM_CLOSE, 0, 0);
    }
}
