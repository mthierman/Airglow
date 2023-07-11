#include "MainWindow.hxx"

Config* MainWindow::pConfig{nullptr};
WebView* MainWindow::pWebView{nullptr};

MainWindow::MainWindow(HINSTANCE hinstance, int ncs, Config* config) {}

std::unique_ptr<MainWindow> MainWindow::Create(HINSTANCE hinstance, int ncs, Config* config)
{
    auto mainWindow{std::unique_ptr<MainWindow>(new MainWindow(hinstance, ncs, config))};

    mainWindow->pConfig = config;

    auto className{to_wide("airglow")};
    auto menuName{to_wide("airglowmenu")};
    auto programIcon{to_wide("PROGRAM_ICON")};
    auto appName{to_wide("Airglow")};
    auto hbrBackground{(HBRUSH)GetStockObject(BLACK_BRUSH)};
    auto hCursor{(HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED)};
    auto hIcon{(HICON)LoadImageW(hinstance, programIcon.c_str(), IMAGE_ICON, 0, 0,
                                 LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED)};

    config->hbrBackground = hbrBackground;
    config->hIcon = hIcon;
    mainWindow->hbrBackground = hbrBackground;
    mainWindow->hIcon = hIcon;

    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpszClassName = className.c_str();
    wcex.lpszMenuName = menuName.c_str();
    wcex.lpfnWndProc = MainWindow::_WndProc;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hinstance;
    wcex.hbrBackground = hbrBackground;
    wcex.hCursor = hCursor;
    wcex.hIcon = hIcon;
    wcex.hIconSm = hIcon;

    if (!RegisterClassExW(&wcex))
        return nullptr;

    mainWindow->hwnd = CreateWindowExW(0, className.c_str(), appName.c_str(), WS_OVERLAPPEDWINDOW,
                                       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                       nullptr, nullptr, hinstance, mainWindow.get());

    config->hwnd = mainWindow->hwnd;

    if (!mainWindow->hwnd)
        return nullptr;

    mainWindow->Show();

    auto webView{WebView::Create(mainWindow->pConfig)};
    if (!webView)
    {
        error("WebView2 creation failed");
        return nullptr;
    }

    mainWindow->pWebView = webView.get();

    return mainWindow;
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

__int64 __stdcall MainWindow::_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    MainWindow* pMainWindow = InstanceFromWndProc<MainWindow>(hwnd, msg, lparam);

    if (pMainWindow)
    {
        switch (msg)
        {

        case WM_ACTIVATE:
            return pMainWindow->_OnActivate(hwnd, wparam, lparam);
        case WM_CHAR:
            return pMainWindow->_OnChar(hwnd, wparam, lparam);
        case WM_CLOSE:
            return pMainWindow->_OnClose(hwnd, wparam, lparam);
        case WM_COMMAND:
            return pMainWindow->_OnCommand(hwnd, wparam, lparam);
        case WM_CREATE:
            return pMainWindow->_OnCreate(hwnd, wparam, lparam);
        case WM_DESTROY:
            return pMainWindow->_OnDestroy(hwnd, wparam, lparam);
        case WM_DPICHANGED:
            return pMainWindow->_OnDpiChanged(hwnd, wparam, lparam);
        case WM_ENTERSIZEMOVE:
            return pMainWindow->_OnEnterSizeMove(hwnd, wparam, lparam);
        case WM_EXITSIZEMOVE:
            return pMainWindow->_OnExitSizeMove(hwnd, wparam, lparam);
        case WM_GETMINMAXINFO:
            return pMainWindow->_OnGetMinMaxInfo(hwnd, wparam, lparam);
        case WM_KEYDOWN:
            return pMainWindow->_OnKeyDown(hwnd, wparam, lparam);
        case WM_MOVE:
            return pMainWindow->_OnMove(hwnd, wparam, lparam);
        case WM_MOVING:
            return pMainWindow->_OnMoving(hwnd, wparam, lparam);
        case WM_PAINT:
            return pMainWindow->_OnPaint(hwnd, wparam, lparam);
        case WM_SETICON:
            return pMainWindow->_OnSetIcon(hwnd, wparam, lparam);
        case WM_SETFOCUS:
            return pMainWindow->_OnSetFocus(hwnd, wparam, lparam);
        // case WM_SETTEXT:
        //     return pMainWindow->_OnSetText(hwnd, wparam, lparam);
        case WM_SETTINGCHANGE:
            return pMainWindow->_OnSettingChange(hwnd, wparam, lparam);
        case WM_SIZE:
            return pMainWindow->_OnSize(hwnd, wparam, lparam);
        case WM_SIZING:
            return pMainWindow->_OnSizing(hwnd, wparam, lparam);
            // case WM_WINDOWPOSCHANGING:
            //     return pMainWindow->_OnWindowPosChanging(hwnd, wparam, lparam);
            // case WM_WINDOWPOSCHANGED:
            //     return pMainWindow->_OnWindowPosChanged(hwnd, wparam, lparam);
        }
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void MainWindow::Show()
{
    Cloak();

    SetDarkTitle();
    SetDarkMode();
    SetMica();

    if (pConfig->settings.position[0] != 0 && pConfig->settings.position[1] != 0 &&
        pConfig->settings.position[2] != 0 && pConfig->settings.position[3] != 0)
    {
        SetWindowPos(hwnd, nullptr, pConfig->settings.position[0], pConfig->settings.position[1],
                     pConfig->settings.position[2], pConfig->settings.position[3], 0);
    }

    if (!pConfig->settings.maximized)
    {

        ShowWindow(hwnd, SW_SHOWDEFAULT);
    }

    if (pConfig->settings.maximized)
    {
        ShowWindow(hwnd, SW_MAXIMIZE);
    }

    if (pConfig->settings.topmost)
    {
        Topmost();
    }

    if (pConfig->settings.fullscreen)
    {
        Fullscreen();
    }

    Uncloak();
}

void MainWindow::Fullscreen()
{
    static RECT position;

    auto style = GetWindowLongPtrW(hwnd, GWL_STYLE);
    if (style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi = {sizeof(mi)};
        GetWindowRect(hwnd, &position);
        if (GetMonitorInfoW(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi))
        {
            SetWindowLongPtrW(hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED);
        }
    }

    else
    {
        SetWindowLongPtrW(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(hwnd, nullptr, position.left, position.top, (position.right - position.left),
                     (position.bottom - position.top), SWP_FRAMECHANGED);
        SendMessage(hwnd, WM_SETICON, 0, 0);
    }
}

void MainWindow::Topmost()
{
    FLASHWINFO fwi{};
    fwi.cbSize = sizeof(FLASHWINFO);
    fwi.hwnd = hwnd;
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 100;

    auto style = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
    if (style & WS_EX_TOPMOST)
    {
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
    }

    else
    {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
    }

    pWebView->SetWindowTitle();
}

void MainWindow::Maximize()
{
    if (!pConfig->settings.fullscreen)
    {
        WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
        GetWindowPlacement(hwnd, &wp);
        if (wp.showCmd == 3)
        {
            ShowWindow(hwnd, SW_SHOWNORMAL);
            SetWindowPos(hwnd, nullptr, pConfig->settings.position[0],
                         pConfig->settings.position[1], pConfig->settings.position[2],
                         pConfig->settings.position[3], 0);
        }

        else
            ShowWindow(hwnd, SW_MAXIMIZE);
    }
}

void MainWindow::SavePosition()
{
    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    GetWindowPlacement(hwnd, &wp);
    if (!pConfig->settings.fullscreen & (wp.showCmd != 3))
    {
        RECT rect{0, 0, 0, 0};
        GetWindowRect(hwnd, &rect);
        pConfig->settings.position = rect_to_bounds(rect);
    }
}

bool MainWindow::CheckSystemDarkMode()
{
    using namespace winrt::Windows::UI;
    using namespace winrt::Windows::UI::ViewManagement;

    UISettings settings{UISettings()};
    Color fg{settings.GetColorValue(UIColorType::Foreground)};

    return (((5 * fg.G) + (2 * fg.R) + fg.B) > (8 * 128));
}

bool MainWindow::SetDarkTitle()
{
    using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode appMode);

    auto uxtheme{LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

    if (!uxtheme)
        return false;

    auto ord135{GetProcAddress(uxtheme, PCSTR MAKEINTRESOURCEW(135))};

    if (!ord135)
        return false;

    auto SetPreferredAppMode{reinterpret_cast<fnSetPreferredAppMode>(ord135)};
    SetPreferredAppMode(PreferredAppMode::AllowDark);
    FreeLibrary(uxtheme);

    return true;
}

bool MainWindow::SetDarkMode()
{
    auto dark{TRUE};
    auto light{FALSE};

    if (!CheckSystemDarkMode())
    {
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &light, sizeof(light));

        return false;
    }

    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));

    return true;
}

bool MainWindow::SetMica()
{
    MARGINS m{0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};
    auto backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;

    if (FAILED(DwmExtendFrameIntoClientArea(hwnd, &m)))
        return false;

    if (FAILED(
            DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(&backdrop))))
        return false;

    return true;
}

bool MainWindow::Cloak()
{
    auto cloak{TRUE};

    if (FAILED(DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloak, sizeof(cloak))))
        return false;

    return true;
}

bool MainWindow::Uncloak()
{
    auto uncloak{FALSE};

    if (FAILED(DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &uncloak, sizeof(uncloak))))
        return false;

    return true;
}

int MainWindow::_OnActivate(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_ACTIVATE");
#endif
    pConfig->Save();

    return 0;
}

int MainWindow::_OnChar(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_CHAR");
#endif

    return 0;
}

int MainWindow::_OnClose(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_CLOSE");
#endif
    DestroyWindow(hwnd);

    return 0;
}

int MainWindow::_OnCommand(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_COMMAND");
#endif

    return 0;
}

int MainWindow::_OnCreate(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_CREATE");
#endif

    return 0;
}

int MainWindow::_OnDestroy(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_DESTROY");
#endif
    PostQuitMessage(0);

    return 0;
}

int MainWindow::_OnDpiChanged(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_DPICHANGED");
#endif

    return 0;
}

int MainWindow::_OnEnterSizeMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_ENTERSIZEMOVE");
#endif

    return 0;
}

int MainWindow::_OnExitSizeMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_EXITSIZEMOVE");
#endif
    SavePosition();

    pConfig->Save();

    return 0;
}

int MainWindow::_OnGetMinMaxInfo(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_GETMINMAXINFO");
#endif
    SavePosition();

    pConfig->Save();

    LPMINMAXINFO minmax = (LPMINMAXINFO)lparam;
    minmax->ptMinTrackSize.x = 300;
    minmax->ptMinTrackSize.y = 300;

    return 0;
}

int MainWindow::_OnKeyDown(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (wparam == VK_F1)
    {
#ifdef _DEBUG
        println("F1");
#endif
        pConfig->settings.split = bool_toggle(pConfig->settings.split);
        pWebView->UpdateBounds();
        pWebView->UpdateFocus();
        pWebView->SetWindowTitle();
        pWebView->SetWindowIcon();
        pConfig->Save();
    }

    if (wparam == VK_F2)
    {
#ifdef _DEBUG
        println("F2");
#endif
        pConfig->settings.swapped = bool_toggle(pConfig->settings.swapped);
        pWebView->UpdateBounds();
        pWebView->UpdateFocus();
        pWebView->SetWindowTitle();
        pWebView->SetWindowIcon();
        pConfig->Save();
    }

    if (wparam == VK_F4)
    {
#ifdef _DEBUG
        println("F4");
#endif
        pConfig->settings.menu = bool_toggle(pConfig->settings.menu);
        pWebView->UpdateBounds();
        pWebView->UpdateFocus();
        pWebView->SetWindowTitle();
        pWebView->SetWindowIcon();
        pConfig->Save();
    }

    if (wparam == VK_F6)
    {
#ifdef _DEBUG
        println("F6");
#endif
        Maximize();
    }

    if (wparam == VK_F11)
    {
#ifdef _DEBUG
        println("F11");
#endif
        pConfig->settings.fullscreen = bool_toggle(pConfig->settings.fullscreen);
        pConfig->Save();

        Fullscreen();
    }

    if (wparam == VK_F9)
    {
#ifdef _DEBUG
        println("F9");
#endif
        pConfig->settings.topmost = bool_toggle(pConfig->settings.topmost);
        pConfig->Save();

        Topmost();
    }

    if (wparam == 0x57)
    {
#ifdef _DEBUG
        println("Ctrl+W");
#endif
        auto state = GetKeyState(VK_CONTROL);
        if (state & 0x8000)
            PostMessageW(hwnd, WM_CLOSE, 0, 0);
    }

    return 0;
}

int MainWindow::_OnMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_MOVE");
#endif

    return 0;
}

int MainWindow::_OnMoving(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_MOVING");
#endif

    return 0;
}

int MainWindow::_OnPaint(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_PAINT");
#endif
    PAINTSTRUCT ps{};
    HDC hdc = BeginPaint(hwnd, &ps);
    EndPaint(hwnd, &ps);

    return 0;
}

int MainWindow::_OnSetIcon(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_SETICON");
#endif
    pWebView->SetWindowIcon();

    return 0;
}

int MainWindow::_OnSetFocus(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_SETFOCUS");
#endif
    if (!pWebView)
        return 0;

    pWebView->UpdateFocus();

    return 0;
}

int MainWindow::_OnSetText(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_SETTEXT");
#endif

    return 0;
}

int MainWindow::_OnSettingChange(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_SETTINGCHANGE");
#endif
    InvalidateRect(hwnd, nullptr, true);
    SetDarkMode();

    return 0;
}

int MainWindow::_OnSize(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_SIZE");
#endif
    if (wparam != 2)
        pConfig->settings.maximized = false;

    if (wparam == 2)
    {
        pConfig->settings.maximized = true;
    }

    pConfig->Save();

    if (!pWebView)
        return 0;

    pWebView->UpdateBounds();

    return 0;
}

int MainWindow::_OnSizing(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_SIZING");
#endif

    return 0;
}

int MainWindow::_OnWindowPosChanged(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_WINDOWPOSCHANGED");
#endif

    return 0;
}

int MainWindow::_OnWindowPosChanging(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_WINDOWPOSCHANGING");
#endif

    return 0;
}
