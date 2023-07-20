#include "App.hxx"

App::App(HINSTANCE hinstance, int ncs) {}

App::~App() { GdiplusShutdown(gdiplusToken); }

std::unique_ptr<App> App::Create(HINSTANCE hinstance, int ncs)
{
    auto app{std::unique_ptr<App>(new App(hinstance, ncs))};

    app->LoadSettings();

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

json App::SerializeJson(Settings s)
{
    try
    {
        return json{{"settings",
                     {
                         {"theme", s.theme},
                         {"mainUrl", s.mainUrl},
                         {"sideUrl", s.sideUrl},
                         {"position", s.position},
                         {"menu", s.menu},
                         {"split", s.split},
                         {"swapped", s.swapped},
                         {"maximized", s.maximized},
                         {"fullscreen", s.fullscreen},
                         {"topmost", s.topmost},
                     }}};
    }
    catch (const std::exception& e)
    {
        println(e.what());
        return json{};
    }
}

Settings App::DeserializeJson(json j)
{
    try
    {
        return Settings{j["settings"]["theme"].get<string>(),
                        j["settings"]["mainUrl"].get<string>(),
                        j["settings"]["sideUrl"].get<string>(),
                        j["settings"]["position"].get<std::vector<int>>(),
                        j["settings"]["menu"].get<bool>(),
                        j["settings"]["split"].get<bool>(),
                        j["settings"]["swapped"].get<bool>(),
                        j["settings"]["maximized"].get<bool>(),
                        j["settings"]["fullscreen"].get<bool>(),
                        j["settings"]["topmost"].get<bool>()

        };
    }
    catch (const std::exception& e)
    {
        println(e.what());
        return Settings{};
    }
}

json App::LoadJson()
{
    try
    {
        ifstream f(paths.json);
        json j{json::parse(f, nullptr, false, true)};
        f.close();

        return j;
    }
    catch (const std::exception& e)
    {
        println(e.what());
        return json{};
    }
}

void App::SaveJson(json j)
{
    try
    {
        ofstream f(paths.json);
        f << std::setw(4) << j << "\n";
        f.close();
    }
    catch (const std::exception& e)
    {
        println(e.what());
        return;
    }
}

void App::LoadSettings()
{
    if (!std::filesystem::exists(paths.json))
        SaveJson(SerializeJson(settings));

    if (std::filesystem::exists(paths.json) && std::filesystem::is_empty(paths.json))
        SaveJson(SerializeJson(settings));

    if (std::filesystem::exists(paths.json))
    {
        auto j{LoadJson()};
        settings = DeserializeJson(j);
    }
}

void App::SaveSettings() { SaveJson(SerializeJson(settings)); }

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
    if (!settings.maximized && !settings.fullscreen)
        settings.position = window_position(hwnd);
    SaveSettings();

    return 0;
}

int App::_OnClose(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    auto style = GetWindowLongPtrW(hwnd, GWL_STYLE);
    auto exStyle = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);

    // if (style & WS_OVERLAPPEDWINDOW)
    //     settings.fullscreen = false;
    // else
    //     settings.fullscreen = true;

    // if (exStyle & WS_EX_TOPMOST)
    //     settings.topmost = true;
    // else
    //     settings.topmost = false;

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
    SaveSettings();

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
        settings.split = bool_toggle(settings.split);
        browser->Bounds(window, settings);
        browser->Focus(window, settings);
        browser->Title(window, settings);
        browser->Icon(window, settings);
        SaveSettings();

        return 0;

    case VK_F2:
        settings.swapped = bool_toggle(settings.swapped);
        browser->Bounds(window, settings);
        browser->Focus(window, settings);
        browser->Title(window, settings);
        browser->Icon(window, settings);
        SaveSettings();

        return 0;

    case VK_F4:
        println("TEST");
        settings.menu = bool_toggle(settings.menu);
        browser->Bounds(window, settings);
        browser->Focus(window, settings);
        browser->Title(window, settings);
        browser->Icon(window, settings);
        SaveSettings();

        return 0;

    case VK_F6:
        settings.maximized = window_maximize(hwnd);
        SaveSettings();

        return 0;

    case VK_F9:
        settings.topmost = window_topmost(hwnd);
        browser->Title(window, settings);
        SaveSettings();

        return 0;

    case VK_F11:
        settings.fullscreen = window_fullscreen(hwnd);
        browser->Icon(window, settings);
        SaveSettings();

        return 0;

    case 0x57:
        auto state = GetKeyState(VK_CONTROL);
        if (state & 0x8000)
            PostMessageW(hwnd, WM_CLOSE, 0, 0);

        return 0;
    }

    return 0;
}

int App::_OnNotify(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    SaveSettings();

    return 0;
}

int App::_OnSetIcon(HWND hwnd, WPARAM wparam, LPARAM lparam) { return 0; }

int App::_OnSetFocus(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    browser->Focus(window, settings);

    return 0;
}

int App::_OnSettingChange(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    settings.theme = window_theme(hwnd);
    colors = Colors{};
    browser->PostSettings(SerializeJson(settings));
    browser->PostSettings(colors.Serialize());
    SaveSettings();

    return 0;
}

int App::_OnSize(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    browser->Bounds(window, settings);

    if (wparam == 2)
        settings.maximized = true;
    else
        settings.maximized = false;

    return 0;
}
