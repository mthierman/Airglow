#include "MainWindow.hxx"

MainWindow::MainWindow(HINSTANCE hinstance, int ncs, Config* config) {}

std::unique_ptr<MainWindow> MainWindow::Create(HINSTANCE hinstance, int ncs, Config* config)
{
    auto mainWindow{std::unique_ptr<MainWindow>(new MainWindow(hinstance, ncs, config))};

    mainWindow->pConfig = config;

    wstring className{L"airglow"};
    wstring menuName{L"airglowmenu"};
    wstring programIcon{L"PROGRAM_ICON"};
    wstring appName{L"Airglow"};

    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpszClassName = className.c_str();
    wcex.lpszMenuName = menuName.c_str();
    wcex.lpfnWndProc = MainWindow::_WndProc;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hinstance;
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.hCursor = (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    wcex.hIcon = (HICON)LoadImageW(hinstance, programIcon.c_str(), IMAGE_ICON, 0, 0,
                                   LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
    wcex.hIconSm = (HICON)LoadImageW(hinstance, programIcon.c_str(), IMAGE_ICON, 0, 0,
                                     LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);

    if (!RegisterClassExW(&wcex))
        return nullptr;

    mainWindow->pConfig->hwnd = CreateWindowExW(
        0, className.c_str(), appName.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hinstance, mainWindow.get());

    if (!mainWindow->pConfig->hwnd)
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
        case WM_COMMAND:
            return pMainWindow->_OnCommand(wparam, lparam);
        case WM_CREATE:
            return pMainWindow->_OnCreate(wparam, lparam);
        case WM_ACTIVATE:
            return pMainWindow->_OnActivate(wparam, lparam);
        case WM_CLOSE:
            return pMainWindow->_OnClose(wparam, lparam);
        case WM_DESTROY:
            return pMainWindow->_OnDestroy(wparam, lparam);
        case WM_DPICHANGED:
            return pMainWindow->_OnDpiChanged(wparam, lparam);
        case WM_GETMINMAXINFO:
            return pMainWindow->_OnGetMinMaxInfo(wparam, lparam);
        case WM_PAINT:
            return pMainWindow->_OnPaint(wparam, lparam);
        case WM_SIZE:
            return pMainWindow->_OnSize(wparam, lparam);
        case WM_SIZING:
            return pMainWindow->_OnSizing(wparam, lparam);
        case WM_ENTERSIZEMOVE:
            return pMainWindow->_OnEnterSizeMove(wparam, lparam);
        case WM_EXITSIZEMOVE:
            return pMainWindow->_OnExitSizeMove(wparam, lparam);
        case WM_MOVE:
            return pMainWindow->_OnMove(wparam, lparam);
        case WM_MOVING:
            return pMainWindow->_OnMoving(wparam, lparam);
        case WM_WINDOWPOSCHANGING:
            return pMainWindow->_OnWindowPosChanging(wparam, lparam);
        case WM_WINDOWPOSCHANGED:
            return pMainWindow->_OnWindowPosChanged(wparam, lparam);
        case WM_SETFOCUS:
            return pMainWindow->_OnSetFocus(wparam, lparam);
        case WM_SETTINGCHANGE:
            return pMainWindow->_OnSettingChange(wparam, lparam);
        case WM_KEYDOWN:
            return pMainWindow->_OnKeyDown(wparam, lparam);
        case WM_CHAR:
            return pMainWindow->_OnChar(wparam, lparam);
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

    if (!pConfig->settings.fullscreen & !pConfig->settings.maximized)
    {
        SetWindowPos(pConfig->hwnd, nullptr, pConfig->settings.position[0],
                     pConfig->settings.position[1], pConfig->settings.position[2],
                     pConfig->settings.position[3], 0);
        ShowWindow(pConfig->hwnd, SW_SHOWNORMAL);
    }

    if (!pConfig->settings.fullscreen & pConfig->settings.maximized)
        ShowWindow(pConfig->hwnd, SW_MAXIMIZE);

    if (pConfig->settings.fullscreen)
    {
        SetWindowPos(pConfig->hwnd, nullptr, pConfig->settings.position[0],
                     pConfig->settings.position[1], pConfig->settings.position[2],
                     pConfig->settings.position[3], 0);
        ShowWindow(pConfig->hwnd, SW_SHOWNORMAL);
        Fullscreen();
    }

    if (pConfig->settings.topmost)
    {
        Topmost();
    }

    else
    {
        SetWindowPos(pConfig->hwnd, nullptr, pConfig->settings.position[0],
                     pConfig->settings.position[1], pConfig->settings.position[2],
                     pConfig->settings.position[3], 0);
        ShowWindow(pConfig->hwnd, SW_SHOWDEFAULT);
    }

    Uncloak();
}

void MainWindow::Fullscreen()
{
    static RECT position;

    auto style = GetWindowLongPtrW(pConfig->hwnd, GWL_STYLE);
    if (style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi = {sizeof(mi)};
        GetWindowRect(pConfig->hwnd, &position);
        if (GetMonitorInfoW(MonitorFromWindow(pConfig->hwnd, MONITOR_DEFAULTTONEAREST), &mi))
        {
            SetWindowLongPtrW(pConfig->hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(pConfig->hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }

    else
    {
        SetWindowLongPtrW(pConfig->hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(pConfig->hwnd, nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        SetWindowPos(pConfig->hwnd, nullptr, position.left, position.top,
                     (position.right - position.left), (position.bottom - position.top), 0);
    }
}

void MainWindow::Topmost()
{
    FLASHWINFO fwi{};
    fwi.cbSize = sizeof(FLASHWINFO);
    fwi.hwnd = pConfig->hwnd;
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 100;

    auto style = GetWindowLongPtrW(pConfig->hwnd, GWL_EXSTYLE);
    if (style & WS_EX_TOPMOST)
    {
        SetWindowPos(pConfig->hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
    }

    else
    {
        SetWindowPos(pConfig->hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
    }
}

bool MainWindow::CheckSystemDarkMode()
{
    using namespace winrt::Windows::UI;
    using namespace winrt::Windows::UI::ViewManagement;

    UISettings settings = UISettings();
    Color fg = settings.GetColorValue(UIColorType::Foreground);

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
        DwmSetWindowAttribute(pConfig->hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &light, sizeof(light));

        return false;
    }

    DwmSetWindowAttribute(pConfig->hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));

    return true;
}

bool MainWindow::SetMica()
{
    MARGINS m{0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};
    auto backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;

    if (FAILED(DwmExtendFrameIntoClientArea(pConfig->hwnd, &m)))
        return false;

    if (FAILED(DwmSetWindowAttribute(pConfig->hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop,
                                     sizeof(&backdrop))))
        return false;

    return true;
}

bool MainWindow::Cloak()
{
    auto cloak = TRUE;

    if (FAILED(DwmSetWindowAttribute(pConfig->hwnd, DWMWA_CLOAK, &cloak, sizeof(cloak))))
        return false;

    return true;
}

bool MainWindow::Uncloak()
{
    auto uncloak = FALSE;

    if (FAILED(DwmSetWindowAttribute(pConfig->hwnd, DWMWA_CLOAK, &uncloak, sizeof(uncloak))))
        return false;

    return true;
}

int MainWindow::_OnActivate(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_ACTIVATE");
#endif
    pConfig->Save();

    return 0;
}

int MainWindow::_OnChar(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_CHAR");
#endif

    return 0;
}

int MainWindow::_OnClose(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_CLOSE");
#endif
    DestroyWindow(pConfig->hwnd);

    return 0;
}

int MainWindow::_OnCommand(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_COMMAND");
#endif

    return 0;
}

int MainWindow::_OnCreate(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_CREATE");
#endif

    return 0;
}

int MainWindow::_OnDestroy(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_DESTROY");
#endif
    PostQuitMessage(0);

    return 0;
}

int MainWindow::_OnDpiChanged(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_DPICHANGED");
#endif

    return 0;
}

int MainWindow::_OnEnterSizeMove(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_ENTERSIZEMOVE");
#endif

    return 0;
}

int MainWindow::_OnExitSizeMove(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_EXITSIZEMOVE");
#endif
    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    GetWindowPlacement(pConfig->hwnd, &wp);
    if (!pConfig->settings.fullscreen & (wp.showCmd != 3))
    {
        RECT rect{0, 0, 0, 0};
        GetWindowRect(pConfig->hwnd, &rect);
        pConfig->settings.position = rect_to_bounds(rect);
    }

    pConfig->Save();

    return 0;
}

int MainWindow::_OnGetMinMaxInfo(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_GETMINMAXINFO");
#endif
    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    GetWindowPlacement(pConfig->hwnd, &wp);
    if (wp.showCmd != 3)
    {
        pConfig->settings.maximized = false;
        pConfig->Save();
    }

    LPMINMAXINFO minmax = (LPMINMAXINFO)lparam;
    minmax->ptMinTrackSize.x = 300;
    minmax->ptMinTrackSize.y = 300;

    return 0;
}

int MainWindow::_OnKeyDown(WPARAM wparam, LPARAM lparam)
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
        if (!pConfig->settings.fullscreen)
        {
            WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
            GetWindowPlacement(pConfig->hwnd, &wp);
            if (wp.showCmd == 3)
            {
                ShowWindow(pConfig->hwnd, SW_SHOWNORMAL);
                SetWindowPos(pConfig->hwnd, nullptr, pConfig->settings.position[0],
                             pConfig->settings.position[1], pConfig->settings.position[2],
                             pConfig->settings.position[3], 0);
            }

            else
                ShowWindow(pConfig->hwnd, SW_MAXIMIZE);
        }
    }

    if (wparam == VK_F11)
    {
#ifdef _DEBUG
        println("F11");
#endif
        pConfig->settings.fullscreen = bool_toggle(pConfig->settings.fullscreen);
        Fullscreen();
        pWebView->UpdateBounds();
        pConfig->Save();
    }

    if (wparam == VK_F9)
    {
#ifdef _DEBUG
        println("F9");
#endif
        pConfig->settings.topmost = bool_toggle(pConfig->settings.topmost);
        Topmost();
        pWebView->SetWindowTitle();
        pConfig->Save();
    }

    if (wparam == 0x57)
    {
#ifdef _DEBUG
        println("Ctrl+W");
#endif
        auto state = GetKeyState(VK_CONTROL);
        if (state & 0x8000)
            PostMessageW(pConfig->hwnd, WM_CLOSE, 0, 0);
    }

    return 0;
}

int MainWindow::_OnMove(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_MOVE");
#endif

    return 0;
}

int MainWindow::_OnMoving(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_MOVING");
#endif

    return 0;
}

int MainWindow::_OnPaint(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_PAINT");
#endif
    PAINTSTRUCT ps{};
    RECT bounds{};
    HDC hdc = BeginPaint(pConfig->hwnd, &ps);
    GetClientRect(pConfig->hwnd, &bounds);
    FillRect(hdc, &bounds, (HBRUSH)GetStockObject(BLACK_BRUSH));
    EndPaint(pConfig->hwnd, &ps);

    return 0;
}

int MainWindow::_OnSetFocus(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_SETFOCUS");
#endif
    if (pWebView)
        pWebView->UpdateFocus();

    return 0;
}

int MainWindow::_OnSettingChange(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_SETTINGCHANGE");
#endif
    InvalidateRect(pConfig->hwnd, nullptr, true);
    SetDarkMode();

    return 0;
}

int MainWindow::_OnSize(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_SIZE");
#endif
    if (wparam == 2)
    {
        pConfig->settings.maximized = true;
        pConfig->Save();
    }

    return 0;
}

int MainWindow::_OnSizing(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_SIZING");
#endif

    return 0;
}

int MainWindow::_OnWindowPosChanged(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_WINDOWPOSCHANGED");
#endif
    pWebView->UpdateBounds();

    return 0;
}

int MainWindow::_OnWindowPosChanging(WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_WINDOWPOSCHANGING");
#endif

    return 0;
}
