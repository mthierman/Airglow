long long __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT bounds;
        GetClientRect(hwnd, &bounds);
        FillRect(hdc, &bounds, (HBRUSH)GetStockObject(BLACK_BRUSH));
        EndPaint(hwnd, &ps);
    }
    break;

    case WM_SETTINGCHANGE:
    {
        systemDarkMode = CheckSystemDarkMode();
        InvalidateRect(hwnd, nullptr, true);
        darkMode = SetDarkMode(hwnd);
    }
    break;

    case WM_SIZE:
    case WM_WINDOWPOSCHANGING:
    {
        RECT windowSize;
        GetClientRect(hwnd, &windowSize);
        auto height = std::to_wstring(windowSize.bottom - windowSize.top);
        auto width = std::to_wstring(windowSize.right - windowSize.left);
        auto dimensions = L"WINDOW SIZE: " + width + L" x " + height + L"\n";

        OutputDebugStringW(dimensions.c_str());

        if (wv_controller != nullptr)
        {
            RECT bounds;
            GetClientRect(hwnd, &bounds);
            RECT wvRect = {
                bounds.left,
                bounds.top,
                bounds.right / 2,
                bounds.bottom,
            };
            if (!isSplit)
                wv_controller->put_Bounds(bounds);
            if (isSplit)
                wv_controller->put_Bounds(wvRect);
        }
        if (wv_controller2 != nullptr)
        {
            RECT bounds2;
            GetClientRect(hwnd, &bounds2);
            RECT wvRect2 = {
                bounds2.right / 2,
                bounds2.top,
                bounds2.right,
                bounds2.bottom,
            };
            if (isSplit)
                wv_controller2->put_Bounds(wvRect2);
        }
    }
    break;

    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO minmax = (LPMINMAXINFO)lp;
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
        if (wp == vkKeyMax)
        {
            KeyMaximize(hwnd);
        }
    }
    break;

    case WM_KEYDOWN:
    {
        if (wp == vkKeyTop)
        {
            KeyTop(hwnd);
        }
        if (wp == vkKeyFull)
        {
            KeyFullscreen(hwnd);
        }
        if (wp == vkKeyW)
        {
            auto state = GetKeyState(vkKeyControl);
            if (state & 0x8000)
            {
                KeyClose(hwnd);
            }
        }
        if (wp == vkKeyS)
        {
            auto state = GetKeyState(vkKeyControl);
            if (state & 0x8000)
            {
                KeySplit(hwnd);
            }
        }
    }
    break;

    case WM_CLOSE:
    {
        DestroyWindow(hwnd);
    }
    break;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;

    default:
        return DefWindowProcW(hwnd, msg, wp, lp);
    }

    return 0;
}
