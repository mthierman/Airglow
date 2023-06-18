__int64 __stdcall WndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        DebugMessages(window);
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
        SendMessageW(window, WM_SETFOCUS, 0, 0);
    }
    break;
    case WM_SETFOCUS:
    {
        UpdateFocus();
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
        UpdateBounds(window);
        UpdateWindowState(window);
        SetWindowTitle(window);
        SetWindowIcon(window);
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
            SendMessageW(window, WM_SETFOCUS, 0, 0);
        }

        if (wparam == VK_F2)
        {
            swapped = SwapPanel();
            SendMessageW(window, WM_SIZE, 0, 0);
            SendMessageW(window, WM_SETFOCUS, 0, 0);
        }

        if (wparam == VK_F4)
        {
            menu = HideMenu();
            SendMessageW(window, WM_SIZE, 0, 0);
            SendMessageW(window, WM_SETFOCUS, 0, 0);
        }

        if (wparam == VK_F6)
        {
            maximized = WindowMaximize(window);
            SendMessageW(window, WM_SIZE, 0, 0);
            SendMessageW(window, WM_SETFOCUS, 0, 0);
        }

        if (wparam == VK_F11)
        {
            fullscreen = WindowFullscreen(window);
            SendMessageW(window, WM_SIZE, 0, 0);
            SendMessageW(window, WM_SETFOCUS, 0, 0);
        }

        if (wparam == VK_F9)
        {
            ontop = WindowTop(window);
            SendMessageW(window, WM_SIZE, 0, 0);
            SendMessageW(window, WM_SETFOCUS, 0, 0);
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
