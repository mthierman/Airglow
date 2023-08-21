#include "App.hxx"

App::App(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
    : storage(std::make_unique<Storage>()), appHwnd(create_window(hInstance)),
      webviewGui(std::make_unique<WebView>(storage.get(), appHwnd, "gui")),
      webviewBar(std::make_unique<WebView>(storage.get(), appHwnd, "bar")),
      webviewMain(std::make_unique<WebView>(storage.get(), appHwnd, "main")),
      webviewSide(std::make_unique<WebView>(storage.get(), appHwnd, "side"))
{
    if (!storage)
        util::error("Storage failed to initialize");

    if (!webviewGui || !webviewBar || !webviewMain || !webviewSide)
        util::error("WebView failed to initialize");

    storage->settings.appScale =
        static_cast<float>(GetDpiForWindow(appHwnd)) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);

    scaledBar = 65 * storage->settings.appScale;

    show_window();
}

App::~App() {}

HWND App::create_window(HINSTANCE hInstance)
{
    storage->application.hIcon =
        (HICON)LoadImageW(hInstance, util::to_wide(storage->settings.appName).c_str(), IMAGE_ICON,
                          0, 0, LR_DEFAULTSIZE);

    auto appName{util::to_wide(storage->settings.appName)};

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = appName.c_str();
    wcex.lpszMenuName = appName.c_str();
    wcex.lpfnWndProc = App::WndProc;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hbrBackground = 0;
    wcex.hCursor = storage->application.hCursor;
    wcex.hIcon = storage->application.hIcon;
    wcex.hIconSm = storage->application.hIcon;

    auto atom{RegisterClassExW(&wcex)};

    if (atom == 0)
        util::error("Window failed to register");

    auto hwnd{CreateWindowExW(0, appName.c_str(), appName.c_str(), WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                              nullptr, hInstance, this)};

    if (!hwnd)
        util::error("Window failed to initialize");

    return hwnd;
}

HWND App::get_hwnd() { return appHwnd; }

void App::show_window()
{
    util::window_darktitle();
    storage->settings.appTheme = util::window_theme(appHwnd);

    auto mica{util::window_mica(appHwnd)};

    if (!mica)
        util::window_cloak(appHwnd);

    if (storage->settings.windowTopmost)
        util::window_topmost(appHwnd);

    if (storage->settings.windowPosition[0] == 0 && storage->settings.windowPosition[1] == 0 &&
        storage->settings.windowPosition[2] == 0 && storage->settings.windowPosition[3] == 0)
    {
        ShowWindow(appHwnd, SW_SHOWDEFAULT);

        if (!mica)
            util::window_uncloak(appHwnd);

        return;
    }

    else
    {
        SetWindowPos(appHwnd, nullptr, storage->settings.windowPosition[0],
                     storage->settings.windowPosition[1], storage->settings.windowPosition[2],
                     storage->settings.windowPosition[3], 0);
    }

    if (storage->settings.windowMaximized)
        ShowWindow(appHwnd, SW_SHOWMAXIMIZED);

    else
        ShowWindow(appHwnd, SW_SHOWNORMAL);

    if (storage->settings.windowFullscreen)
        util::window_fullscreen(appHwnd);

    if (!mica)
        util::window_uncloak(appHwnd);
}

void App::resized()
{
    if (!webviewGui && !webviewBar && !webviewMain && !webviewSide)
        return;

    if (!webviewGui->controller || !webviewBar->controller || !webviewMain->controller ||
        !webviewSide->controller)
        return;

    auto emptyRect{winrt::Rect{0, 0, 0, 0}};

    RECT bounds{0, 0, 0, 0};
    GetClientRect(appHwnd, &bounds);

    if (storage->settings.webviewGui)
    {
        webviewMain->controller.Bounds(emptyRect);
        webviewSide->controller.Bounds(emptyRect);
        webviewGui->controller.Bounds(panel_gui(bounds));
        webviewBar->controller.Bounds(emptyRect);

        return;
    }

    else
    {
        if (!storage->settings.webviewSplit && !storage->settings.webviewSwapped)
        {
            webviewMain->controller.Bounds(panel_full(bounds));
            webviewSide->controller.Bounds(emptyRect);
            webviewGui->controller.Bounds(emptyRect);
            webviewBar->controller.Bounds(panel_bar(bounds));

            return;
        }

        if (!storage->settings.webviewSplit && storage->settings.webviewSwapped)
        {
            webviewMain->controller.Bounds(emptyRect);
            webviewSide->controller.Bounds(panel_full(bounds));
            webviewGui->controller.Bounds(emptyRect);
            webviewBar->controller.Bounds(panel_bar(bounds));

            return;
        }

        if (!storage->settings.webviewHorizontal)
        {
            if (storage->settings.webviewSplit && !storage->settings.webviewSwapped)
            {
                webviewMain->controller.Bounds(panel_left(bounds));
                webviewSide->controller.Bounds(panel_right(bounds));
                webviewGui->controller.Bounds(emptyRect);
                webviewBar->controller.Bounds(panel_bar(bounds));

                return;
            }

            if (storage->settings.webviewSplit && storage->settings.webviewSwapped)
            {
                webviewMain->controller.Bounds(panel_right(bounds));
                webviewSide->controller.Bounds(panel_left(bounds));
                webviewGui->controller.Bounds(emptyRect);
                webviewBar->controller.Bounds(panel_bar(bounds));

                return;
            }
        }

        if (storage->settings.webviewHorizontal)
        {
            if (storage->settings.webviewSplit && !storage->settings.webviewSwapped)
            {
                webviewMain->controller.Bounds(panel_top(bounds));
                webviewSide->controller.Bounds(panel_bot(bounds));
                webviewGui->controller.Bounds(emptyRect);
                webviewBar->controller.Bounds(panel_bar(bounds));

                return;
            }

            if (storage->settings.webviewSplit && storage->settings.webviewSwapped)
            {
                webviewMain->controller.Bounds(panel_bot(bounds));
                webviewSide->controller.Bounds(panel_top(bounds));
                webviewGui->controller.Bounds(emptyRect);
                webviewBar->controller.Bounds(panel_bar(bounds));

                return;
            }
        }
    }
}

winrt::Windows::Foundation::Rect App::panel_gui(RECT bounds)
{
    return winrt::Windows::Foundation::Rect{
        static_cast<float>(bounds.left), static_cast<float>(bounds.top),
        static_cast<float>(bounds.right), static_cast<float>(bounds.bottom)};
}

winrt::Windows::Foundation::Rect App::panel_bar(RECT bounds)
{
    return winrt::Windows::Foundation::Rect{
        static_cast<float>(bounds.left), static_cast<float>(bounds.bottom - scaledBar),
        static_cast<float>(bounds.right - bounds.left), static_cast<float>(scaledBar)};
}

winrt::Windows::Foundation::Rect App::panel_full(RECT bounds)
{
    return winrt::Windows::Foundation::Rect{
        static_cast<float>(bounds.left), static_cast<float>(bounds.top),
        static_cast<float>(bounds.right - bounds.left),
        static_cast<float>((bounds.bottom - bounds.top) - scaledBar)};
}

winrt::Windows::Foundation::Rect App::panel_left(RECT bounds)
{
    return winrt::Windows::Foundation::Rect{
        static_cast<float>(bounds.left), static_cast<float>(bounds.top),
        static_cast<float>(bounds.right / 2),
        static_cast<float>((bounds.bottom - bounds.top) - scaledBar)};
}

winrt::Windows::Foundation::Rect App::panel_right(RECT bounds)
{
    return winrt::Windows::Foundation::Rect{
        static_cast<float>(bounds.right / 2), static_cast<float>(bounds.top),
        static_cast<float>(bounds.right / 2),
        static_cast<float>((bounds.bottom - bounds.top) - scaledBar)};
}

winrt::Windows::Foundation::Rect App::panel_top(RECT bounds)
{
    return winrt::Windows::Foundation::Rect{
        static_cast<float>(bounds.left), static_cast<float>(bounds.top),
        static_cast<float>(bounds.right),
        static_cast<float>((bounds.bottom / 2) - (scaledBar / 2))};
}

winrt::Windows::Foundation::Rect App::panel_bot(RECT bounds)
{
    return winrt::Windows::Foundation::Rect{
        static_cast<float>(bounds.left), static_cast<float>((bounds.bottom / 2) - (scaledBar / 2)),
        static_cast<float>(bounds.right),
        static_cast<float>((bounds.bottom / 2) - (scaledBar / 2))};
}

__int64 __stdcall App::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    App* app = InstanceFromWndProc<App>(hwnd, msg, lparam);

    if (app)
    {
        switch (msg)
        {
        case WM_ACTIVATE:
            return app->wm_activate(hwnd, msg, wparam, lparam);
        case WM_CLOSE:
            return app->wm_close(hwnd, msg, wparam, lparam);
        case WM_DESTROY:
            return app->wm_destroy(hwnd, msg, wparam, lparam);
        case WM_DEVTOOLSMAIN:
            return app->wm_devtoolsmain(hwnd, msg, wparam, lparam);
        case WM_DEVTOOLSSIDE:
            return app->wm_devtoolsside(hwnd, msg, wparam, lparam);
        case WM_DPICHANGED:
            return app->wm_dpichanged(hwnd, msg, wparam, lparam);
        case WM_ERASEBKGND:
            return app->wm_erasebkgnd(hwnd, msg, wparam, lparam);
        case WM_EXITSIZEMOVE:
            return app->wm_exitsizemove(hwnd, msg, wparam, lparam);
        case WM_GETMINMAXINFO:
            return app->wm_getminmaxinfo(hwnd, msg, wparam, lparam);
        case WM_HOMEMAIN:
            return app->wm_homemain(hwnd, msg, wparam, lparam);
        case WM_HOMESIDE:
            return app->wm_homeside(hwnd, msg, wparam, lparam);
        case WM_KEYDOWN:
            return app->wm_keydown(hwnd, msg, wparam, lparam);
        case WM_NAVIGATEMAIN:
            return app->wm_navigatemain(hwnd, msg, wparam, lparam);
        case WM_NAVIGATESIDE:
            return app->wm_navigateside(hwnd, msg, wparam, lparam);
        case WM_NOTIFY:
            return app->wm_notify(hwnd, msg, wparam, lparam);
        case WM_SETTINGCHANGE:
            return app->wm_settingchange(hwnd, msg, wparam, lparam);
        case WM_WINDOWPOSCHANGED:
            return app->wm_windowposchanged(hwnd, msg, wparam, lparam);
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

int App::wm_activate(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (!storage->settings.windowMaximized && !storage->settings.windowFullscreen)
    {
        storage->settings.windowPosition = util::window_position(hwnd);
        storage->save();
    }

    return 0;
}

int App::wm_close(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    storage->save();

    DestroyWindow(hwnd);

    return 0;
}

int App::wm_destroy(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    PostQuitMessage(0);

    return 0;
}

int App::wm_devtoolsmain(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (!webviewMain || !webviewMain->core)
        return 0;

    webviewMain->core.OpenDevToolsWindow();

    return 0;
}

int App::wm_devtoolsside(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (!webviewSide || !webviewSide->core)
        return 0;

    webviewSide->core.OpenDevToolsWindow();

    return 0;
}

int App::wm_dpichanged(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    storage->settings.appScale =
        static_cast<float>(GetDpiForWindow(hwnd)) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);

    scaledBar = 65 * storage->settings.appScale;

    auto bounds{(RECT*)lparam};

    SetWindowPos(appHwnd, nullptr, bounds->left, bounds->top, (bounds->right - bounds->left),
                 (bounds->bottom - bounds->top), SWP_NOZORDER | SWP_NOACTIVATE);

    return 0;
}

int App::wm_erasebkgnd(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    InvalidateRect(hwnd, nullptr, true);
    PAINTSTRUCT ps{};
    HDC hdc = BeginPaint(hwnd, &ps);

    if (storage->settings.appTheme == "dark")
        FillRect(hdc, &ps.rcPaint, storage->application.darkBrush);

    else
        FillRect(hdc, &ps.rcPaint, storage->application.lightBrush);

    EndPaint(hwnd, &ps);

    return 1;
}

int App::wm_exitsizemove(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (!storage->settings.windowMaximized && !storage->settings.windowFullscreen)
    {
        storage->settings.windowPosition = util::window_position(hwnd);
        storage->save();
    }

    return 0;
}

int App::wm_getminmaxinfo(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO minmax{(LPMINMAXINFO)lparam};
    minmax->ptMinTrackSize.x = 500;
    minmax->ptMinTrackSize.y = 500;

    return 0;
}

int App::wm_homemain(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (!webviewMain->core || !webviewMain->controller)
        return 0;

    webviewMain->core.Navigate(winrt::to_hstring(storage->settings.mainHomepage));

    return 0;
}

int App::wm_homeside(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (!webviewSide->core || !webviewSide->controller)
        return 0;

    webviewSide->core.Navigate(winrt::to_hstring(storage->settings.sideHomepage));

    return 0;
}

int App::wm_keydown(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (!webviewGui || !webviewBar || !webviewMain || !webviewSide)
        return 0;

    switch (wparam)
    {
    case VK_PAUSE:
    {
        storage->settings.webviewGui = storage->settings.webviewGui ? false : true;

        break;
    }

    case 0x4C:
    {
        if (GetKeyState(VK_CONTROL) & 0x8000)
            webviewBar->focus();

        break;
    }

    case 0x57:
    {
        if (GetKeyState(VK_CONTROL) & 0x8000)
            PostMessageW(hwnd, WM_CLOSE, 0, 0);

        break;
    }

    case VK_F1:
    {
        storage->settings.webviewSwapped = storage->settings.webviewSwapped ? false : true;

        break;
    }

    case VK_F2:
    {
        storage->settings.webviewSplit = storage->settings.webviewSplit ? false : true;

        break;
    }

    case VK_F3:
    {
        storage->settings.webviewHorizontal = storage->settings.webviewHorizontal ? false : true;

        break;
    }

    case VK_F4:
    {
        if (GetKeyState(VK_MENU) & 0x8000)
            SendMessageW(hwnd, WM_CLOSE, 0, 0);

        else
        {
            storage->settings.windowTopmost = util::window_topmost(hwnd);
        }

        break;
    }

    case VK_F6:
    {
        storage->settings.windowMaximized = util::window_maximize(hwnd);

        break;
    }

    case VK_F8:
    {
        webviewMain->core.Navigate(winrt::to_hstring(storage->settings.mainHomepage));
        webviewSide->core.Navigate(winrt::to_hstring(storage->settings.sideHomepage));

        break;
    }

    case VK_F11:
    {
        storage->settings.windowFullscreen = util::window_fullscreen(hwnd);

        break;
    }

    default:
        return 0;
    }

    SendMessageW(hwnd, WM_NOTIFY, 0, 0);

    return 0;
}

int App::wm_navigatemain(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (!webviewMain->core || !webviewMain->controller)
        return 0;

    webviewMain->core.Navigate(winrt::to_hstring(storage->settings.mainCurrentPage));

    return 0;
}

int App::wm_navigateside(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (!webviewSide->core || !webviewSide->controller)
        return 0;

    webviewSide->core.Navigate(winrt::to_hstring(storage->settings.sideCurrentPage));

    return 0;
}

int App::wm_notify(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (webviewGui && webviewBar && webviewMain && webviewSide)
    {
        webviewGui->post_settings(storage->serialize());
        webviewBar->post_settings(storage->serialize());
        webviewMain->post_settings(storage->serialize());
        webviewSide->post_settings(storage->serialize());

        if (storage->settings.webviewGui)
        {
            // webviewGui->focus();
            webviewGui->title();
            webviewGui->icon();
        }

        else
        {
            // webviewMain->focus();
            webviewMain->title();
            webviewMain->icon();

            // webviewSide->focus();
            webviewSide->title();
            webviewSide->icon();
        }

        resized();

        storage->save();
    }

    return 0;
}

int App::wm_settingchange(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    storage->settings.appTheme = util::window_theme(hwnd);
    storage->colors = Colors{};

    SendMessageW(hwnd, WM_NOTIFY, 0, 0);

    return 0;
}

int App::wm_windowposchanged(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    GetWindowPlacement(hwnd, &wp);
    wp.showCmd == 3 ? storage->settings.windowMaximized = true
                    : storage->settings.windowMaximized = false;

    resized();

    return 0;
}
