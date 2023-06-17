void TestingResize(HWND window)
{
    auto dpi = GetDpiForWindow(window);
    auto test = std::to_wstring(dpi);
    OutputDebugStringW(L"DPI: ");
    OutputDebugStringW(test.c_str());
    OutputDebugStringW(L"\n");

    RECT bounds;
    GetClientRect(window, &bounds);
    auto height = std::to_wstring(bounds.bottom - bounds.top);
    auto width = std::to_wstring(bounds.right - bounds.left);
    auto dimensions = L"WINDOW SIZE: " + width + L" x " + height + L"\n";
    OutputDebugStringW(dimensions.c_str());
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
        // if (wv_controller != nullptr & wv_controller2 != nullptr)
        // {
        //     if (!swapped)
        //         wv_controller->MoveFocus(
        //             COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        //     if (swapped)
        //         wv_controller2->MoveFocus(
        //             COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        // }
    }
    break;
    case WM_SIZE:
    case WM_WINDOWPOSCHANGING:
    {
        // TestingResize(window);
        if (wv_controller != nullptr)
        {
            wv_controller->put_Bounds(GetMainPanelBounds(window));
        }
        if (wv_controller2 != nullptr)
        {
            wv_controller2->put_Bounds(GetSidePanelBounds(window));
        }
        // if (wv_controller3 != nullptr)
        // {
        //     wv_controller3->put_Bounds(GetMenuBounds(window));
        // }
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
    case WM_SYSKEYDOWN:
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
            // menu = PanelHideMenu(window);
        }
        if (wparam == VK_F10)
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
            {
                SendMessageW(window, WM_CLOSE, 0, 0);
            }
        }
    }
    break;
    case WM_CLOSE:
    {
        Shutdown(window);
        Gdiplus::GdiplusShutdown(gdiplusToken);
        DestroyWindow(window);
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
