#include "App.hxx"

App::App(HINSTANCE hinstance, int ncs) {}

App::~App() { GdiplusShutdown(gdiplusToken); }

std::unique_ptr<App> App::Create(HINSTANCE hinstance, int ncs)
{
    auto app{std::unique_ptr<App>(new App(hinstance, ncs))};

    SetEnvironmentVariableW(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR", L"0");

    if (!std::filesystem::exists(app->paths.json))
        app->SaveSettings();

    if (std::filesystem::exists(app->paths.json) && std::filesystem::is_empty(app->paths.json))
        app->SaveSettings();

    if (std::filesystem::exists(app->paths.json))
    {
        auto j{app->settings.Load()};
        app->settings = app->settings.Deserialize(j);
    }

    app->window.icon = (HICON)LoadImageW(hinstance, to_wide("PROGRAM_ICON").c_str(), IMAGE_ICON, 0,
                                         0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = app->window.name.c_str();
    wcex.lpszMenuName = app->window.name.c_str();
    wcex.lpfnWndProc = App::_WndProc;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hinstance;
    wcex.hbrBackground = 0;
    wcex.hCursor = app->window.cursor;
    wcex.hIcon = app->window.icon;
    wcex.hIconSm = app->window.icon;

    if (!RegisterClassExW(&wcex))
        return nullptr;

    app->window.hwnd = CreateWindowExW(
        0, app->window.name.c_str(), app->window.name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hinstance, app.get());

    if (!app->window.hwnd)
        return nullptr;

    app->Show();

    app->browser = Browser::Create(app->window, app->settings, app->colors);

    if (!app->browser)
        return nullptr;

    if (GdiplusStartup(&app->gdiplusToken, &app->gdiplusStartupInput, nullptr) !=
        Gdiplus::Status::Ok)
        return nullptr;

    return app;
}

void App::Show()
{
    window_cloak(window.hwnd);
    window_darktitle();
    settings.theme = window_theme(window.hwnd);
    window_mica(window.hwnd);

    if (settings.position[0] == 0 && settings.position[1] == 0 && settings.position[2] == 0 &&
        settings.position[3] == 0)
        ShowWindow(window.hwnd, SW_SHOWDEFAULT);

    else
    {
        if (settings.maximized)
        {
            SetWindowPos(window.hwnd, nullptr, settings.position[0], settings.position[1],
                         settings.position[2], settings.position[3], 0);
            ShowWindow(window.hwnd, SW_SHOWMAXIMIZED);
        }

        else
        {
            SetWindowPos(window.hwnd, nullptr, settings.position[0], settings.position[1],
                         settings.position[2], settings.position[3], 0);
            ShowWindow(window.hwnd, SW_SHOWDEFAULT);
        }
    }

    if (settings.topmost)
    {
        window_topmost(window.hwnd);
    }

    window_uncloak(window.hwnd);

    if (settings.fullscreen)
    {
        window_fullscreen(window.hwnd);
    }
}

void App::SaveSettings() { settings.Save(settings.Serialize()); }

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

__int64 __stdcall App::_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    App* app = InstanceFromWndProc<App>(hwnd, msg, lparam);

    if (app)
    {
        switch (msg)
        {
        case WM_ACTIVATE:
            return app->_OnActivate(hwnd, wparam, lparam);
        case WM_CLOSE:
            return app->_OnClose(hwnd, wparam, lparam);
        case WM_DESTROY:
            return app->_OnDestroy(hwnd, wparam, lparam);
        case WM_ERASEBKGND:
            return app->_OnEraseBackground(hwnd, wparam, lparam);
        case WM_EXITSIZEMOVE:
            return app->_OnExitSizeMove(hwnd, wparam, lparam);
        case WM_GETMINMAXINFO:
            return app->_OnGetMinMaxInfo(hwnd, wparam, lparam);
        case WM_KEYDOWN:
            return app->_OnKeyDown(hwnd, wparam, lparam);
        case WM_NOTIFY:
            return app->_OnNotify(hwnd, wparam, lparam);
        case WM_SETFOCUS:
            return app->_OnSetFocus(hwnd, wparam, lparam);
        case WM_SETTINGCHANGE:
            return app->_OnSettingChange(hwnd, wparam, lparam);
        case WM_WINDOWPOSCHANGED:
            return app->_OnWindowPosChanged(hwnd, wparam, lparam);
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

int App::_OnActivate(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (!settings.maximized && !settings.fullscreen)
        settings.position = window_position(hwnd);

    SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);

    return 0;
}

int App::_OnClose(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    SaveSettings();

    DestroyWindow(hwnd);

    return 0;
}

int App::_OnDestroy(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    PostQuitMessage(0);

    return 0;
}

int App::_OnEraseBackground(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    InvalidateRect(hwnd, nullptr, true);
    PAINTSTRUCT ps{};
    HDC hdc = BeginPaint(hwnd, &ps);

    if (settings.theme == "dark")
        FillRect(hdc, &ps.rcPaint, window.darkBrush);

    else
        FillRect(hdc, &ps.rcPaint, window.lightBrush);

    EndPaint(hwnd, &ps);

    return 1;
}

int App::_OnExitSizeMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (!settings.maximized && !settings.fullscreen)
        settings.position = window_position(hwnd);

    SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);

    return 0;
}

int App::_OnGetMinMaxInfo(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO minmax = (LPMINMAXINFO)lparam;
    minmax->ptMinTrackSize.x = 500;
    minmax->ptMinTrackSize.y = 500;

    return 0;
}

int App::_OnKeyDown(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    switch (wparam)
    {
    case VK_PAUSE:
        settings.menu = bool_toggle(settings.menu);
        SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);

        return 0;

    case 0x57:
        if (GetKeyState(VK_CONTROL) & 0x8000)
            PostMessageW(hwnd, WM_CLOSE, 0, 0);

        return 0;

    case VK_F1:
        settings.swapped = bool_toggle(settings.swapped);
        SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);

        return 0;

    case VK_F2:
        settings.split = bool_toggle(settings.split);
        SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);

        return 0;

    case VK_F3:
        settings.horizontal = bool_toggle(settings.horizontal);
        SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);

        return 0;

    case VK_F4:
        if (GetKeyState(VK_MENU) & 0x8000)
        {
            SendMessageW(hwnd, WM_CLOSE, 0, 0);
        }

        else
        {
            settings.topmost = window_topmost(hwnd);
            SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);
        }

        return 0;

    case VK_F6:
        settings.maximized = window_maximize(hwnd);
        SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);

        return 0;

    case VK_F8:
        browser->NavigateHome(settings);

        return 0;

    case VK_F11:
        settings.fullscreen = window_fullscreen(hwnd);
        SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);

        return 0;
    }

    return 0;
}

int App::_OnNotify(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    println("OnNotify");

    browser->PostSettings(settings.Serialize());
    browser->PostSettings(colors.Serialize());

    browser->Bounds(window, settings);
    browser->Focus(window, settings);
    browser->Title(window, settings);
    browser->Icon(window, settings);

    SaveSettings();

    return 0;
}

int App::_OnSetFocus(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    browser->Focus(window, settings);

    return 0;
}

int App::_OnSettingChange(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    settings.theme = window_theme(hwnd);
    colors = Colors{};
    SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);

    return 0;
}

int App::_OnWindowPosChanged(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    browser->Bounds(window, settings);

    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    GetWindowPlacement(hwnd, &wp);
    if (wp.showCmd == 3)
        settings.maximized = true;
    else
        settings.maximized = false;

    SaveSettings();

    return 0;
}
