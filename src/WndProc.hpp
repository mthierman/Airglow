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
        SendMessageW(window, WM_SETFOCUS, 0, 0);
    }
    break;
    case WM_SETFOCUS:
    {
        SetWindowFocus(window);
    }
    break;
    case WM_WINDOWPOSCHANGING:
    {
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
            PanelSplit(window);
        }

        if (wparam == VK_F2)
        {
            PanelSwap(window);
            SetWindowTitle(window);
            SetWindowIcon(window);
        }

        if (wparam == VK_F4)
        {
            PanelHideMenu(window);
            SetWindowTitle(window);
            SetWindowIcon(window);
        }

        if (wparam == VK_F5)
        {
            WindowMaximize(window);
        }

        if (wparam == VK_F11)
        {
            WindowFullscreen(window);
        }

        if (wparam == VK_F9)
        {
            WindowTop(window);
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

void WebViewMessages(HWND window, PWSTR message)
{
    if ((std::wstring)message == std::wstring(L"F1").c_str())
    {
        PanelSplit(window);
    }

    if ((std::wstring)message == std::wstring(L"F2").c_str())
    {
        PanelSwap(window);
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if ((std::wstring)message == std::wstring(L"F4").c_str())
    {
        PanelHideMenu(window);
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if ((std::wstring)message == std::wstring(L"F5").c_str())
    {
        WindowMaximize(window);
    }

    if ((std::wstring)message == std::wstring(L"F11").c_str())
    {
        WindowFullscreen(window);
    }

    if ((std::wstring)message == std::wstring(L"F9").c_str())
    {
        WindowTop(window);
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if ((std::wstring)message == std::wstring(L"close").c_str())
    {
        SendMessageW(window, WM_CLOSE, 0, 0);
    }
}
