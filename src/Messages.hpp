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
        UpdateBounds(window);
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
            split = Toggle(split);
            UpdateBounds(window);
            UpdateFocus();
            SetWindowTitle(window);
            SetWindowIcon(window);
        }

        if (wparam == VK_F2)
        {
            swapped = Toggle(swapped);
            UpdateBounds(window);
            UpdateFocus();
            SetWindowTitle(window);
            SetWindowIcon(window);
        }

        if (wparam == VK_F4)
        {
            menu = Toggle(menu);
            UpdateBounds(window);
            UpdateFocus();
            SetWindowTitle(window);
            SetWindowIcon(window);
        }

        if (wparam == VK_F6)
        {
            maximized = Toggle(maximized);
            MaximizeWindow(window);
            UpdateFocus();
        }

        if (wparam == VK_F11)
        {
            fullscreen = Toggle(fullscreen);
            FullscreenWindow(window);
            UpdateFocus();
        }

        if (wparam == VK_F9)
        {
            topmost = Toggle(topmost);
            TopmostWindow(window);
            UpdateFocus();
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
        split = Toggle(split);
        UpdateBounds(window);
        UpdateFocus();
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if (message == swapKey)
    {
        swapped = Toggle(swapped);
        UpdateBounds(window);
        UpdateFocus();
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if (message == hideMenuKey)
    {
        menu = Toggle(menu);
        UpdateBounds(window);
        UpdateFocus();
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if (message == maximizeKey)
    {
        maximized = Toggle(maximized);
        MaximizeWindow(window);
        UpdateFocus();
    }

    if (message == fullscreenKey)
    {
        fullscreen = Toggle(fullscreen);
        FullscreenWindow(window);
        UpdateFocus();
    }

    if (message == onTopKey)
    {
        topmost = Toggle(topmost);
        TopmostWindow(window);
        UpdateFocus();
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if (message == closeKey)
    {
        SendMessageW(window, WM_CLOSE, 0, 0);
    }
}
