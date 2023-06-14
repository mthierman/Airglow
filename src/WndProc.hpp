__int3264 __stdcall WndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
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
        systemDarkMode = CheckSystemDarkMode();
        InvalidateRect(window, nullptr, true);
        darkMode = SetDarkMode(window);
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
            RECT wvRect2 = {
                bounds.right / 2,
                bounds.top,
                bounds.right,
                bounds.bottom,
            };
            if (isSplit)
                wv_controller2->put_Bounds(wvRect2);
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
        if (wparam == vkKeyMax)
        {
            KeyMaximize(window);
        }
    }
    break;

    case WM_KEYDOWN:
    {
        if (wparam == vkKeyTop)
        {
            KeyTop(window);
        }
        if (wparam == vkKeyFull)
        {
            KeyFullscreen(window);
        }
        if (wparam == vkKeyW)
        {
            auto state = GetKeyState(vkKeyControl);
            if (state & 0x8000)
            {
                KeyClose(window);
            }
        }
        if (wparam == vkKeyS)
        {
            auto state = GetKeyState(vkKeyControl);
            if (state & 0x8000)
            {
                KeySplit(window);
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
