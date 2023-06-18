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

    case WM_WINDOWPOSCHANGED:
    {
        OutputDebugStringW(L"WM_WINDOWPOSCHANGED");
        UpdateBounds(window);
    }
    break;

        // case WM_ACTIVATE:
        // {
        //     OutputDebugStringW(L"WM_ACTIVATE");
        //     SendMessageW(window, WM_SETFOCUS, 0, 0);
        // }
        // break;

        // case WM_SETFOCUS:
        // {
        //     OutputDebugStringW(L"WM_SETFOCUS");
        //     UpdateFocus();
        // }
        // break;

        // case WM_SIZE:
        // {
        //     OutputDebugStringW(L"WM_SIZE");
        // }
        // break;

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
            split = ToggleSplit();
            UpdateBounds(window);
            UpdateFocus();
            SetWindowTitle(window);
            SetWindowIcon(window);
        }

        if (wparam == VK_F2)
        {
            swapped = ToggleSwap();
            UpdateBounds(window);
            UpdateFocus();
            SetWindowTitle(window);
            SetWindowIcon(window);
        }

        if (wparam == VK_F4)
        {
            menu = ToggleMenu();
            UpdateBounds(window);
            UpdateFocus();
            SetWindowTitle(window);
            SetWindowIcon(window);
        }

        if (wparam == VK_F6)
        {
            maximized = ToggleMaximize();
            MaximizeWindow(window);
        }

        if (wparam == VK_F11)
        {
            fullscreen = ToggleFullscreen();
            FullscreenWindow(window);
        }

        if (wparam == VK_F9)
        {
            topmost = ToggleTopmost();
            TopmostWindow(window);
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
        Shutdown();
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

void Messages(std::wstring message)
{
    std::wstring splitKey = std::wstring(L"F1");
    std::wstring swapKey = std::wstring(L"F2");
    std::wstring hideMenuKey = std::wstring(L"F4");
    std::wstring maximizeKey = std::wstring(L"F6");
    std::wstring fullscreenKey = std::wstring(L"F11");
    std::wstring onTopKey = std::wstring(L"F9");
    std::wstring closeKey = std::wstring(L"close");

    if (message == splitKey)
    {
        split = ToggleSplit();
        UpdateBounds(window);
        UpdateFocus();
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if (message == swapKey)
    {
        swapped = ToggleSwap();
        UpdateBounds(window);
        UpdateFocus();
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if (message == hideMenuKey)
    {
        menu = ToggleMenu();
        UpdateBounds(window);
        UpdateFocus();
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if (message == maximizeKey)
    {
        maximized = ToggleMaximize();
        MaximizeWindow(window);
    }

    if (message == fullscreenKey)
    {
        fullscreen = ToggleFullscreen();
        FullscreenWindow(window);
    }

    if (message == onTopKey)
    {
        topmost = ToggleTopmost();
        TopmostWindow(window);
    }

    if (message == closeKey)
    {
        SendMessageW(window, WM_CLOSE, 0, 0);
    }
}
