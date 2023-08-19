#include "App.hxx"

#define WM_NAVIGATEMAIN (WM_APP + 0)
#define WM_NAVIGATESIDE (WM_APP + 1)

App::App(Storage* s, HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
    : storage(s), appHwnd(create_window(hInstance))
{
    show_window();

    webviewMain = std::make_unique<WebView>(storage, appHwnd, "main");
    webviewSide = std::make_unique<WebView>(storage, appHwnd, "side");
    webviewGui = std::make_unique<WebView>(storage, appHwnd, "gui");
    webviewBar = std::make_unique<WebView>(storage, appHwnd, "bar");
}

App::~App() {}

HWND App::create_window(HINSTANCE hInstance)
{
    storage->application.hIcon = (HICON)LoadImageW(hInstance, util::to_wide(APP_NAME).c_str(),
                                                   IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);

    WNDCLASSEXW wcex{sizeof(WNDCLASSEX)};
    wcex.lpszClassName = storage->application.appName.c_str();
    wcex.lpszMenuName = storage->application.appName.c_str();
    wcex.lpfnWndProc = App::_WndProc;
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

    auto hwnd{CreateWindowExW(0, storage->application.appName.c_str(),
                              storage->application.appName.c_str(), WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                              nullptr, hInstance, this)};

    if (!hwnd)
        util::error("Window failed to initialize");

    storage->application.scale =
        static_cast<float>(GetDpiForWindow(hwnd)) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);

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
    }

    else
    {
        if (!storage->settings.webviewSplit && !storage->settings.webviewSwapped)
        {
            webviewMain->controller.Bounds(panel_full(bounds));
            webviewSide->controller.Bounds(emptyRect);
            webviewBar->controller.Bounds(panel_bar(bounds));
        }

        if (!storage->settings.webviewSplit && storage->settings.webviewSwapped)
        {
            webviewMain->controller.Bounds(emptyRect);
            webviewSide->controller.Bounds(panel_full(bounds));
            webviewBar->controller.Bounds(panel_bar(bounds));
        }

        if (!storage->settings.webviewHorizontal)
        {
            if (storage->settings.webviewSplit && !storage->settings.webviewSwapped)
            {
                webviewMain->controller.Bounds(panel_left(bounds));
                webviewSide->controller.Bounds(panel_right(bounds));
                webviewBar->controller.Bounds(panel_bar(bounds));
            }

            if (storage->settings.webviewSplit && storage->settings.webviewSwapped)
            {
                webviewMain->controller.Bounds(panel_right(bounds));
                webviewSide->controller.Bounds(panel_left(bounds));
                webviewBar->controller.Bounds(panel_bar(bounds));
            }
        }

        if (storage->settings.webviewHorizontal)
        {
            if (storage->settings.webviewSplit && !storage->settings.webviewSwapped)
            {
                webviewMain->controller.Bounds(panel_top(bounds));
                webviewSide->controller.Bounds(panel_bot(bounds));
                webviewBar->controller.Bounds(panel_bar(bounds));
            }

            if (storage->settings.webviewSplit && storage->settings.webviewSwapped)
            {
                webviewMain->controller.Bounds(panel_bot(bounds));
                webviewSide->controller.Bounds(panel_top(bounds));
                webviewBar->controller.Bounds(panel_bar(bounds));
            }
        }

        webviewGui->controller.Bounds(emptyRect);
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
    auto scaledBar{50 * storage->application.scale};

    return winrt::Windows::Foundation::Rect{
        static_cast<float>(bounds.left), static_cast<float>(bounds.bottom - scaledBar),
        static_cast<float>(bounds.right - bounds.left), static_cast<float>(scaledBar)};
}

winrt::Windows::Foundation::Rect App::panel_full(RECT bounds)
{
    auto scaledBar{50 * storage->application.scale};

    return winrt::Windows::Foundation::Rect{
        static_cast<float>(bounds.left), static_cast<float>(bounds.top),
        static_cast<float>(bounds.right - bounds.left),
        static_cast<float>((bounds.bottom - bounds.top) - scaledBar)};
}

winrt::Windows::Foundation::Rect App::panel_left(RECT bounds)
{
    auto scaledBar{50 * storage->application.scale};

    return winrt::Windows::Foundation::Rect{
        static_cast<float>(bounds.left), static_cast<float>(bounds.top),
        static_cast<float>(bounds.right / 2),
        static_cast<float>((bounds.bottom - bounds.top) - scaledBar)};
}

winrt::Windows::Foundation::Rect App::panel_right(RECT bounds)
{
    auto scaledBar{50 * storage->application.scale};

    return winrt::Windows::Foundation::Rect{
        static_cast<float>(bounds.right / 2), static_cast<float>(bounds.top),
        static_cast<float>(bounds.right / 2),
        static_cast<float>((bounds.bottom - bounds.top) - scaledBar)};
}

winrt::Windows::Foundation::Rect App::panel_top(RECT bounds)
{
    auto scaledBar{50 * storage->application.scale};

    return winrt::Windows::Foundation::Rect{
        static_cast<float>(bounds.left), static_cast<float>(bounds.top),
        static_cast<float>(bounds.right),
        static_cast<float>((bounds.bottom / 2) - (scaledBar / 2))};
}

winrt::Windows::Foundation::Rect App::panel_bot(RECT bounds)
{
    auto scaledBar{50 * storage->application.scale};

    return winrt::Windows::Foundation::Rect{
        static_cast<float>(bounds.left), static_cast<float>((bounds.bottom / 2) - (scaledBar / 2)),
        static_cast<float>(bounds.right),
        static_cast<float>((bounds.bottom / 2) - (scaledBar / 2))};
}

__int64 __stdcall App::_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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
        case WM_DPICHANGED:
            return app->wm_dpichanged(hwnd, msg, wparam, lparam);
        case WM_ERASEBKGND:
            return app->wm_erasebkgnd(hwnd, msg, wparam, lparam);
        case WM_EXITSIZEMOVE:
            return app->wm_exitsizemove(hwnd, msg, wparam, lparam);
        case WM_GETMINMAXINFO:
            return app->wm_getminmaxinfo(hwnd, msg, wparam, lparam);
        case WM_KEYDOWN:
            return app->wm_keydown(hwnd, msg, wparam, lparam);
        case WM_NAVIGATEMAIN:
            return app->wm_navigatemain(hwnd, msg, wparam, lparam);
        case WM_NAVIGATESIDE:
            return app->wm_navigateside(hwnd, msg, wparam, lparam);
        case WM_NOTIFY:
            return app->wm_notify(hwnd, msg, wparam, lparam);
        case WM_SETFOCUS:
            return app->wm_setfocus(hwnd, msg, wparam, lparam);
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
        storage->settings.windowPosition = util::window_position(hwnd);

    SendMessageW(hwnd, WM_NOTIFY, 0, 0);

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

int App::wm_dpichanged(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    storage->application.scale =
        static_cast<float>(GetDpiForWindow(hwnd)) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);

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
        storage->settings.windowPosition = util::window_position(hwnd);

    SendMessageW(hwnd, WM_NOTIFY, 0, 0);

    return 0;
}

int App::wm_getminmaxinfo(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO minmax{(LPMINMAXINFO)lparam};
    minmax->ptMinTrackSize.x = 500;
    minmax->ptMinTrackSize.y = 500;

    return 0;
}

int App::wm_keydown(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (wparam)
    {
    case VK_PAUSE:
        storage->settings.webviewGui = storage->settings.webviewGui ? false : true;
        SendMessageW(hwnd, WM_NOTIFY, 0, 0);
        // browser->FocusSettings();

        return 0;

    case 0x4C:
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            // browser->FocusBar();
        }

        return 0;

    case 0x57:
        if (GetKeyState(VK_CONTROL) & 0x8000)
            PostMessageW(hwnd, WM_CLOSE, 0, 0);

        return 0;

    case VK_F1:
        storage->settings.webviewSwapped = storage->settings.webviewSwapped ? false : true;
        SendMessageW(hwnd, WM_NOTIFY, 0, 0);

        return 0;

    case VK_F2:
        storage->settings.webviewSplit = storage->settings.webviewSplit ? false : true;
        SendMessageW(hwnd, WM_NOTIFY, 0, 0);

        return 0;

    case VK_F3:
        storage->settings.webviewHorizontal = storage->settings.webviewHorizontal ? false : true;
        SendMessageW(hwnd, WM_NOTIFY, 0, 0);

        return 0;

    case VK_F4:
        if (GetKeyState(VK_MENU) & 0x8000)
        {
            SendMessageW(hwnd, WM_CLOSE, 0, 0);
        }

        else
        {
            storage->settings.windowTopmost = util::window_topmost(hwnd);
            SendMessageW(hwnd, WM_NOTIFY, 0, 0);
        }

        return 0;

    case VK_F6:
        storage->settings.windowMaximized = util::window_maximize(hwnd);
        SendMessageW(hwnd, WM_NOTIFY, 0, 0);

        return 0;

    case VK_F8:
        webviewMain->navigate_home();
        webviewSide->navigate_home();

        return 0;

    case VK_F11:
        storage->settings.windowFullscreen = util::window_fullscreen(hwnd);
        SendMessageW(hwnd, WM_NOTIFY, 0, 0);

        return 0;
    }

    return 0;
}

int App::wm_navigatemain(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (webviewMain->core)
        webviewMain->core.Navigate(winrt::to_hstring(storage->settings.mainCurrentPage));

    return 0;
}

int App::wm_navigateside(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (webviewSide->core)
        webviewSide->core.Navigate(winrt::to_hstring(storage->settings.sideCurrentPage));

    return 0;
}

int App::wm_notify(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (webviewGui && webviewBar && webviewMain && webviewSide)
    {
        resized();

        webviewGui->post_settings(storage->serialize());
        webviewBar->post_settings(storage->serialize());

        webviewGui->title();
        webviewMain->title();
        webviewSide->title();

        webviewGui->icon();
        webviewMain->icon();
        webviewSide->icon();

        SendMessageW(appHwnd, WM_SETFOCUS, 0, 0);
    }

    storage->save();

    return 0;
}

int App::wm_setfocus(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (webviewGui && webviewMain && webviewSide)
    {
        if (storage->settings.webviewGui)
            webviewGui->focus();

        if (!storage->settings.webviewGui && !storage->settings.webviewSwapped)
            webviewMain->focus();

        if (!storage->settings.webviewGui && storage->settings.webviewSwapped)
            webviewSide->focus();
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

    if (webviewGui && webviewBar && webviewMain && webviewSide)
        resized();

    storage->save();

    return 0;
}
