#include "App.hxx"

App::App(HINSTANCE hinstance, int ncs) {}

App::~App() { GdiplusShutdown(gdiplusToken); }

std::unique_ptr<App> App::Create(HINSTANCE hinstance, int ncs)
{
    auto app{std::unique_ptr<App>(new App(hinstance, ncs))};

    app->Load();

    SetEnvironmentVariableW(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR", L"0");

    if (GdiplusStartup(&app->gdiplusToken, &app->gdiplusStartupInput, nullptr) !=
        Gdiplus::Status::Ok)
        return nullptr;

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

    app->Show(app->window.hwnd);

    app->browser = Browser::Create(app->window.hwnd);

    if (!app->browser)
        return nullptr;

    return app;
}

void App::Show(HWND hwnd)
{
    window_cloak(hwnd);
    window_darktitle();
    window.theme = window_theme(hwnd);
    window_mica(hwnd);

    if (window.position[0] == 0 && window.position[1] == 0 && window.position[2] == 0 &&
        window.position[3] == 0)
        ShowWindow(hwnd, SW_SHOWDEFAULT);

    else
    {
        if (window.maximized)
            ShowWindow(hwnd, SW_MAXIMIZE);

        else
        {
            SetWindowPos(hwnd, nullptr, window.position[0], window.position[1], window.position[2],
                         window.position[3], 0);
            ShowWindow(hwnd, SW_SHOWNORMAL);
        }
    }

    if (window.topmost)
        window_topmost(hwnd);

    if (window.fullscreen)
        window_fullscreen(hwnd);

    window_uncloak(hwnd);
}

void App::Load()
{
    using namespace State;

    if (std::filesystem::exists(path.json) && !std::filesystem::is_empty(path.json))
    {
        auto load = window_load_state(path);

        if (load.empty())
            return;

        window = window_deserialize(load);
    }
}

void App::Save()
{
    using namespace State;

    window_save_state(path, json{window_serialize(window)});
}

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
        case WM_SETICON:
            return app->_OnSetIcon(hwnd, wparam, lparam);
        case WM_SETFOCUS:
            return app->_OnSetFocus(hwnd, wparam, lparam);
        case WM_SETTINGCHANGE:
            return app->_OnSettingChange(hwnd, wparam, lparam);
        case WM_SIZE:
            return app->_OnSize(hwnd, wparam, lparam);
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

int App::_OnActivate(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (!window.maximized && !window.fullscreen)
        window.position = window_position(hwnd);
    Save();

    return 0;
}

int App::_OnClose(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    Save();
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

    if (window.theme == "dark")
        FillRect(hdc, &ps.rcPaint, window.darkBrush);

    else
        FillRect(hdc, &ps.rcPaint, window.lightBrush);

    EndPaint(hwnd, &ps);

    return 1;
}

int App::_OnExitSizeMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (!window.maximized && !window.fullscreen)
        window.position = window_position(hwnd);
    Save();

    return 0;
}

int App::_OnGetMinMaxInfo(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO minmax = (LPMINMAXINFO)lparam;
    minmax->ptMinTrackSize.x = 400;
    minmax->ptMinTrackSize.y = 450;

    return 0;
}

int App::_OnKeyDown(HWND hwnd, WPARAM wparam, LPARAM lparam)
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
        window.maximized = window_maximize(hwnd);
        Save();

        return 0;
    case VK_F9:
        window.topmost = window_topmost(hwnd);
        Save();

        return 0;
    case VK_F11:
        window.fullscreen = window_fullscreen(hwnd);
        Save();

        return 0;
    case 0x57:
        auto state = GetKeyState(VK_CONTROL);
        if (state & 0x8000)
            PostMessageW(hwnd, WM_CLOSE, 0, 0);
        return 0;
    }

    return 0;
}

int App::_OnSetIcon(HWND hwnd, WPARAM wparam, LPARAM lparam) { return 0; }

int App::_OnSetFocus(HWND hwnd, WPARAM wparam, LPARAM lparam) { return 0; }

int App::_OnSettingChange(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    window.theme = window_theme(hwnd);
    Save();

    return 0;
}

int App::_OnSize(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (wparam != 2)
        window.maximized = false;

    if (wparam == 2)
    {
        window.maximized = true;
    }
    Save();

    browser->Bounds(window);

    return 0;
}