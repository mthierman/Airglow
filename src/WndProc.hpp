void TestingResize(HWND window)
{
    std::wstring dpi = L"DPI: " + std::to_wstring(GetDpiForWindow(window)) + L"\n";
    OutputDebugStringW(dpi.c_str());

    RECT client;
    GetClientRect(window, &client);
    std::wstring clientRect = L"ClientRect: " + std::to_wstring(client.right - client.left) +
                              L" x " + std::to_wstring(client.bottom - client.top) + L"\n";
    OutputDebugStringW(clientRect.c_str());

    RECT bounds;
    GetWindowRect(window, &bounds);
    std::wstring windowRect = L"WindowRect: " + std::to_wstring(bounds.right - bounds.left) +
                              L" x " + std::to_wstring(bounds.bottom - bounds.top) + L"\n";
    OutputDebugStringW(windowRect.c_str());
}

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
        if (wv_controller != nullptr & wv_controller2 != nullptr)
        {
            if (!swapped)
                wv_controller->MoveFocus(
                    COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

            if (swapped)
                wv_controller2->MoveFocus(
                    COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        }

        if (wv_controller3 != nullptr)
        {
            if (menu)
                wv_controller3->MoveFocus(
                    COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        }
    }
    break;
    case WM_SIZE:
    case WM_WINDOWPOSCHANGING:
    {
        // TestingResize(window);

        if (wv_controller != nullptr)
            wv_controller->put_Bounds(GetMainPanelBounds(window));

        if (wv_controller2 != nullptr)
            wv_controller2->put_Bounds(GetSidePanelBounds(window));

        if (wv_controller3 != nullptr)
            wv_controller3->put_Bounds(GetMenuBounds(window));

        SendMessageW(window, WM_SETFOCUS, 0, 0);
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
            split = PanelSplit(window);
        }

        if (wparam == VK_F2)
        {
            swapped = PanelSwap(window);
            SetWindowTitle(window);
            SetWindowIcon(window);
        }

        if (wparam == VK_F4)
        {
            menu = PanelHideMenu(window);
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
