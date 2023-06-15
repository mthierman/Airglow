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
    case WM_SIZE:
    case WM_WINDOWPOSCHANGING:
    {
        TestingResize(window);
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
            if (!panelMenu)
                wv_controller3->put_Bounds(GetHiddenPanelBounds(window));
            if (panelMenu)
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
    case WM_KEYDOWN:
    {
        if (wparam == VK_F1)
        {
            panelMenu = PanelHideMenu(window);
        }
        if (wparam == VK_F2)
        {
            isSplit = PanelSplit(window);
        }
        if (wparam == VK_F4)
        {
            isMaximized = WindowMaximize(window);
        }
        if (wparam == VK_F11)
        {
            isFullscreen = WindowFullscreen(window);
        }
        if (wparam == VK_F9)
        {
            isTopmost = WindowTop(window);
            SetWindowTitle(window);
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
