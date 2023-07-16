#include "Window.hxx"
#include "Utility.hxx"

using namespace Utility;

std::unique_ptr<Window> Window::Create(HINSTANCE hinstance, int ncs)
{
    auto window{std::unique_ptr<Window>(new Window(hinstance, ncs))};

    window->appName = to_wide(APP_NAME);
    window->appVersion = to_wide(APP_VERSION);
    window->cursor =
        (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    window->darkBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    window->lightBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    window->icon = (HICON)LoadImageW(hinstance, to_wide("PROGRAM_ICON").c_str(), IMAGE_ICON, 0, 0,
                                     LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpszClassName = window->appName.c_str();
    wcex.lpszMenuName = window->appName.c_str();
    wcex.lpfnWndProc = Window::_WndProc;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hinstance;
    wcex.hbrBackground = 0;
    wcex.hCursor = window->cursor;
    wcex.hIcon = window->icon;
    wcex.hIconSm = window->icon;

    if (!RegisterClassExW(&wcex))
        return nullptr;

    window->hwnd = CreateWindowExW(0, window->appName.c_str(), window->appName.c_str(),
                                   WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                   CW_USEDEFAULT, nullptr, nullptr, hinstance, window.get());

    if (!window->hwnd)
    {
        Utility::error("Window creation failed");
        return nullptr;
    }

    window->Show();

    return window;
}

void Window::Show()
{
    window_cloak(hwnd);
    window_allowdark();
    Theme();
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    window_uncloak(hwnd);
}

void Window::Theme() { window_darkmode(hwnd); }

void Window::Maximize() { window_maximize(hwnd); }

void Window::Topmost() { window_topmost(hwnd); }

void Window::Fullscreen()
{
    window_cloak(hwnd);
    window_fullscreen(hwnd);
    window_uncloak(hwnd);
}

Window::Window(HINSTANCE hinstance, int ncs) {}

template <class T> T* InstanceFromWndProc(HWND hwnd, UINT msg, LPARAM lparam)
{
    T* pInstance;

    if (msg == WM_NCCREATE)
    {
        LPCREATESTRUCTW pCreateStruct = reinterpret_cast<LPCREATESTRUCTW>(lparam);
        pInstance = reinterpret_cast<T*>(pCreateStruct->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pInstance));
    }

    else
        pInstance = reinterpret_cast<T*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));

    return pInstance;
}

__int64 __stdcall Window::_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Window* window = InstanceFromWndProc<Window>(hwnd, msg, lparam);

    if (window)
    {
        switch (msg)
        {
        case WM_ACTIVATE:
            return window->_OnActivate(hwnd, wparam, lparam);
        case WM_CLOSE:
            return window->_OnClose(hwnd, wparam, lparam);
        case WM_DESTROY:
            return window->_OnDestroy(hwnd, wparam, lparam);
        case WM_ERASEBKGND:
            return window->_OnEraseBackground(hwnd, wparam, lparam);
        case WM_EXITSIZEMOVE:
            return window->_OnExitSizeMove(hwnd, wparam, lparam);
        case WM_GETMINMAXINFO:
            return window->_OnGetMinMaxInfo(hwnd, wparam, lparam);
        case WM_KEYDOWN:
            return window->_OnKeyDown(hwnd, wparam, lparam);
        case WM_SETICON:
            return window->_OnSetIcon(hwnd, wparam, lparam);
        case WM_SETFOCUS:
            return window->_OnSetFocus(hwnd, wparam, lparam);
        case WM_SETTINGCHANGE:
            return window->_OnSettingChange(hwnd, wparam, lparam);
        case WM_SIZE:
            return window->_OnSize(hwnd, wparam, lparam);
        }
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

int Window::_OnActivate(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (!maximized && !fullscreen)
        position = window_position(hwnd);

    return 0;
}

int Window::_OnClose(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    DestroyWindow(hwnd);

    return 0;
}

int Window::_OnDestroy(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    PostQuitMessage(0);

    return 0;
}

int Window::_OnEraseBackground(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    InvalidateRect(hwnd, nullptr, true);
    PAINTSTRUCT ps{};
    HDC hdc = BeginPaint(hwnd, &ps);

    if (allowdark)
        FillRect(hdc, &ps.rcPaint, darkBrush);

    else
        FillRect(hdc, &ps.rcPaint, lightBrush);

    EndPaint(hwnd, &ps);

    return 1;
}

int Window::_OnExitSizeMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (!maximized && !fullscreen)
        position = window_position(hwnd);

    return 0;
}

int Window::_OnGetMinMaxInfo(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO minmax = (LPMINMAXINFO)lparam;
    minmax->ptMinTrackSize.x = 400;
    minmax->ptMinTrackSize.y = 450;

    return 0;
}

int Window::_OnKeyDown(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    switch (wparam)
    {
    case VK_F1:
        // pConfig->settings.split = bool_toggle(pConfig->settings.split);
        return 0;
    case VK_F2:
        // pConfig->settings.swapped = bool_toggle(pConfig->settings.swapped);
        return 0;
    case VK_F4:
        // pConfig->settings.menu = bool_toggle(pConfig->settings.menu);
        return 0;
    case VK_F6:
        Maximize();
        return 0;
    case VK_F9:
        Topmost();
        return 0;
    case VK_F11:
        Fullscreen();
        return 0;
    case 0x57:
        auto state = GetKeyState(VK_CONTROL);
        if (state & 0x8000)
            PostMessageW(hwnd, WM_CLOSE, 0, 0);
        return 0;
    }

    return 0;
}

int Window::_OnSetIcon(HWND hwnd, WPARAM wparam, LPARAM lparam) { return 0; }

int Window::_OnSetFocus(HWND hwnd, WPARAM wparam, LPARAM lparam) { return 0; }

int Window::_OnSettingChange(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    Theme();

    return 0;
}

int Window::_OnSize(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (wparam != 2)
        maximized = false;

    if (wparam == 2)
    {
        maximized = true;
    }

    // pWebView->UpdateBounds();

    return 0;
}
