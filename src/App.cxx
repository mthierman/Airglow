#include "App.hxx"

App::App(HINSTANCE hinstance, int ncs) {}

App::~App() { GdiplusShutdown(gdiplusToken); }

std::unique_ptr<App> App::Create(HINSTANCE hinstance, int ncs)
{
    auto app{std::unique_ptr<App>(new App(hinstance, ncs))};

    SetEnvironmentVariableW(to_wide("WEBVIEW2_DEFAULT_BACKGROUND_COLOR").c_str(),
                            to_wide("0").c_str());

    if (GdiplusStartup(&app->gdiplusToken, &app->gdiplusStartupInput, nullptr) !=
        Gdiplus::Status::Ok)
        return nullptr;

    app->name = to_wide(APP_NAME);
    app->version = to_wide(APP_VERSION);

    app->paths.data = path_portable();
    app->paths.settings = path_settings();
    app->paths.config = path_json();
    app->paths.db = path_db();
    app->paths.js = path_js();

    app->cursor = (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    app->darkBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    app->lightBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    app->icon = (HICON)LoadImageW(hinstance, to_wide("PROGRAM_ICON").c_str(), IMAGE_ICON, 0, 0,
                                  LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpszClassName = app->name.c_str();
    wcex.lpszMenuName = app->name.c_str();
    wcex.lpfnWndProc = App::_WndProc;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hinstance;
    wcex.hbrBackground = 0;
    wcex.hCursor = app->cursor;
    wcex.hIcon = app->icon;
    wcex.hIconSm = app->icon;

    if (!RegisterClassExW(&wcex))
        return nullptr;

    app->hwnd = CreateWindowExW(0, app->name.c_str(), app->name.c_str(), WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                                nullptr, hinstance, app.get());

    if (!app->hwnd)
        return nullptr;

    app->Show();

    app->browser = Browser::Create(app->hwnd);

    if (!app->browser)
        return nullptr;

    return app;
}

void App::Show()
{
    window_cloak(hwnd);
    window_darktitle();
    theme = window_theme(hwnd);
    window_mica(hwnd);
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    window_uncloak(hwnd);
}

void App::Maximize() { window_maximize(hwnd); }

void App::Topmost() { window_topmost(hwnd); }

void App::Fullscreen()
{
    window_cloak(hwnd);
    window_fullscreen(hwnd);
    window_uncloak(hwnd);
}

// bool App::Environment()
// {
// if (!std::filesystem::exists(paths.config))
//     Save();

// Load();

// colors = GetSystemColors();

// if (!std::filesystem::exists(paths.data) || !std::filesystem::exists(paths.settings) ||
//     !std::filesystem::exists(paths.config))
//     return false;

// return true;
// }

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

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

int App::_OnActivate(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (!maximized && !fullscreen)
        position = window_position(hwnd);

    return 0;
}

int App::_OnClose(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
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

    if (theme == "dark")
        FillRect(hdc, &ps.rcPaint, darkBrush);

    else
        FillRect(hdc, &ps.rcPaint, lightBrush);

    EndPaint(hwnd, &ps);

    return 1;
}

int App::_OnExitSizeMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (!maximized && !fullscreen)
        position = window_position(hwnd);

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

int App::_OnSetIcon(HWND hwnd, WPARAM wparam, LPARAM lparam) { return 0; }

int App::_OnSetFocus(HWND hwnd, WPARAM wparam, LPARAM lparam) { return 0; }

int App::_OnSettingChange(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    theme = window_theme(hwnd);

    return 0;
}

int App::_OnSize(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (wparam != 2)
        maximized = false;

    if (wparam == 2)
    {
        maximized = true;
    }

    browser->Bounds();

    return 0;
}

// std::pair<wstring, wstring> App::args()
// {
//     std::pair<wstring, wstring> commands;

//     auto cmd = GetCommandLineW();
//     int count;

//     auto args = CommandLineToArgvW(cmd, &count);

//     if (count == 2)
//     {
//         commands.first = args[1];
//         commands.second = wstring{};
//     }

//     if (count == 3)
//     {
//         commands.first = args[1];
//         commands.second = args[2];
//     }

//     LocalFree(args);

//     if (!commands.first.empty())
//     {
//         if (!commands.first.starts_with(L"http") || !commands.first.starts_with(L"https"))
//             commands.first = L"https://" + commands.first;
//     }

//     if (!commands.second.empty())
//     {
//         if (!commands.second.starts_with(L"http") || !commands.second.starts_with(L"https"))
//             commands.second = L"https://" + commands.second;
//     }

//     return commands;
// }

// void App::LoadJson()
// {
//     if (std::filesystem::exists(paths.config) && !std::filesystem::is_empty(paths.config))
//     {
//         try
//         {
//             json j{};
//             ifstream f(paths.config);
//             j = json::parse(f, nullptr, false, true);
//             f.close();

//             settings.position = j["position"].get<std::vector<int>>();
//             settings.menu = j["menu"].get<bool>();
//             settings.split = j["split"].get<bool>();
//             settings.swapped = j["swapped"].get<bool>();
//             settings.maximized = j["maximized"].get<bool>();
//             settings.fullscreen = j["fullscreen"].get<bool>();
//             settings.topmost = j["topmost"].get<bool>();
//             settings.mainUrl = j["mainUrl"].get<string>();
//             settings.sideUrl = j["sideUrl"].get<string>();
//         }
//         catch (const std::exception& e)
//         {
//             return;
//         }
//     }
// }

// void App::SaveJson()
// {
//     try
//     {
//         json j{};
//         j["position"] = settings.position;
//         j["menu"] = settings.menu;
//         j["split"] = settings.split;
//         j["swapped"] = settings.swapped;
//         j["maximized"] = settings.maximized;
//         j["fullscreen"] = settings.fullscreen;
//         j["topmost"] = settings.topmost;
//         j["mainUrl"] = settings.mainUrl;
//         j["sideUrl"] = settings.sideUrl;

//         ofstream f(paths.config);
//         f << std::setw(4) << j << "\n";
//         f.close();
//     }
//     catch (const std::exception& e)
//     {
//         return;
//     }
// }

// void App::CreateDb()
// {
//     auto dbFile{(paths.db).string()};
//     auto dbPath{dbFile.c_str()};
//     sqlite3* db{nullptr};
//     char* errMsg{0};
//     string sql{"CREATE TABLE CONFIG("
//                "X INT NOT NULL,"
//                "Y INT NOT NULL,"
//                "WIDTH INT NOT NULL,"
//                "HEIGHT INT NOT NULL,"
//                "MENU INT NOT NULL,"
//                "SPLIT INT NOT NULL,"
//                "MAXIMIZED INT NOT NULL,"
//                "FULLSCREEN INT NOT NULL,"
//                "TOPMOST INT NOT NULL,"
//                "MAIN TEXT NOT NULL,"
//                "SIDE TEXT NOT NULL);"};

//     auto dbOpen = sqlite3_open(dbPath, &db);
//     if (dbOpen != SQLITE_OK)
//     {
//         dberror("Database opening failed");
//         return;
//     }

//     if (std::filesystem::exists(paths.db))
//     {
//         auto debExec = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);
//         if (debExec != SQLITE_OK)
//         {
//             dberror(errMsg);
//             sqlite3_free(errMsg);
//         }
//     }

//     if (!std::filesystem::exists(paths.db))
//         return;

//     sqlite3_close(db);

//     return;
// }

// json Config::GetCurrent()
// {
//     json j{{"settings",
//             {{"mainUrl", pConfig->settings.mainUrl},
//              {"sideUrl", pConfig->settings.sideUrl},
//              {"accentColor", pConfig->settings.accentColor}}}};
// }
