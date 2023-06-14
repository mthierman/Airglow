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
    // case WM_SETICON:
    // {
    //     auto iconTest = 1;
    //     OutputDebugStringW(L"FAVICON CHANGED");
    // }
    // break;
    case WM_SETTINGCHANGE:
    {
        InvalidateRect(window, nullptr, true);
        SetDarkMode(window);
    }
    break;
    case WM_SIZE:
    case WM_WINDOWPOSCHANGING:
    {
        GetClientRect(window, &bounds);
        auto height = std::to_wstring(bounds.bottom - bounds.top);
        auto width = std::to_wstring(bounds.right - bounds.left);
        auto dimensions = L"WINDOW SIZE: " + width + L" x " + height + L"\n";
        OutputDebugStringW(dimensions.c_str());
        if (wv_controller != nullptr)
        {
            RECT wv_bounds = {
                bounds.left,
                bounds.top,
                bounds.right / 2,
                bounds.bottom,
            };
            if (isSplit)
                wv_controller->put_Bounds(wv_bounds);
            if (!isSplit)
                wv_controller->put_Bounds(bounds);
        }
        if (wv_controller2 != nullptr)
        {
            RECT wv_bounds2 = {
                bounds.right / 2,
                bounds.top,
                bounds.right,
                bounds.bottom,
            };
            if (isSplit)
                wv_controller2->put_Bounds(wv_bounds2);
        }
    }
    break;
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO minmax = (LPMINMAXINFO)lparam;
        minmax->ptMinTrackSize.x = 400;
        minmax->ptMinTrackSize.y = GetSystemMetrics(SM_CYCAPTION);
    }
    break;
    case WM_SETFOCUS:
    {
        if (wv_controller != nullptr)
        {
            wv_controller->MoveFocus(
                COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        }
        if (wv_controller2 != nullptr)
        {
            wv_controller2->MoveFocus(
                COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        }
    }
    break;
    case WM_SYSKEYDOWN:
    {
        if (wparam == VK_F1)
        {
            isMaximized = KeyMaximize(window);
        }
    }
    break;
    case WM_KEYDOWN:
    {
        if (wparam == VK_F2)
        {
            isTopmost = KeyTop(window);
        }
        if (wparam == VK_F11)
        {
            isFullscreen = KeyFullscreen(window);
        }
        if (wparam == 0x53)
        {
            auto state = GetKeyState(VK_MENU);
            if (state & 0x8000)
            {
                isSplit = KeySplit(window);
            }
        }
        if (wparam == 0x57)
        {
            auto state = GetKeyState(VK_CONTROL);
            if (state & 0x8000)
            {
                KeyClose(window);
            }
        }
    }
    break;
    case WM_CLOSE:
    {
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
