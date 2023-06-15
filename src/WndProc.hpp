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
    case WM_SIZE:
    case WM_WINDOWPOSCHANGING:
    {
        RECT bounds;
        GetClientRect(window, &bounds);
        auto height = std::to_wstring(bounds.bottom - bounds.top);
        auto width = std::to_wstring(bounds.right - bounds.left);
        auto dimensions = L"WINDOW SIZE: " + width + L" x " + height + L"\n";
        OutputDebugStringW(dimensions.c_str());

        if (wv_controller != nullptr)
        {
            if (!isSplit)
                wv_controller->put_Bounds(GetFullPanelBounds(window));
            if (isSplit)
                wv_controller->put_Bounds(GetLeftPanelBounds(window));
        }

        if (wv_controller2 != nullptr)
        {
            RECT hidden = {0, 0, 0, 0};
            if (!isSplit)
                wv_controller2->put_Bounds(GetHiddenPanelBounds(window));
            if (isSplit)
                wv_controller2->put_Bounds(GetRightPanelBounds(window));
        }

        if (wv_controller3 != nullptr)
        {
            wv_controller3->put_Bounds(GetBottomPanelBounds(window));
        }
    }
    break;
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO minmax = (LPMINMAXINFO)lparam;
        minmax->ptMinTrackSize.x = 800;
        minmax->ptMinTrackSize.y = 600;
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
        if (wv_controller3 != nullptr)
        {
            wv_controller3->MoveFocus(
                COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        }
    }
    break;
    case WM_KEYDOWN:
    {
        if (wparam == VK_F2)
        {
            isMaximized = KeyMaximize(window);
        }
        if (wparam == VK_F4)
        {
            isTopmost = KeyTop(window);
        }
        if (wparam == VK_F11)
        {
            isFullscreen = KeyFullscreen(window);
        }
        if (wparam == VK_F1)
        {
            isSplit = KeySplit(window);
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
