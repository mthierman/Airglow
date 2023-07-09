#include "MainWindow.hxx"
#include "WebView.hxx"

Config* MainWindow::pConfig{nullptr};

MainWindow::MainWindow(HINSTANCE hinstance, int ncs, Config* config) {}

std::unique_ptr<MainWindow> MainWindow::Create(HINSTANCE hinstance, int ncs, Config* config)
{
    pConfig = config;

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

    auto pMainWindow{std::unique_ptr<MainWindow>(new MainWindow(hinstance, ncs, pConfig))};

    pMainWindow->hwnd = CreateWindowExW(0, className.c_str(), appName.c_str(), WS_OVERLAPPEDWINDOW,
                                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                        nullptr, nullptr, hinstance, pMainWindow.get());

    if (!pMainWindow->hwnd)
        return nullptr;

    return pMainWindow;
}

void MainWindow::Show(HWND hwnd)
{
    Cloak(hwnd);

    SetDarkTitle();
    SetDarkMode(hwnd);
    SetMica(hwnd);

    if (!pConfig->fullscreen & !pConfig->maximized)
    {
        SetWindowPos(hwnd, nullptr, pConfig->position[0], pConfig->position[1],
                     pConfig->position[2], pConfig->position[3], 0);
        ShowWindow(hwnd, SW_SHOWNORMAL);
    }

    if (!pConfig->fullscreen & pConfig->maximized)
        ShowWindow(hwnd, SW_MAXIMIZE);

    if (pConfig->fullscreen)
    {
        SetWindowPos(hwnd, nullptr, pConfig->position[0], pConfig->position[1],
                     pConfig->position[2], pConfig->position[3], 0);
        ShowWindow(hwnd, SW_SHOWNORMAL);
        Fullscreen(hwnd);
    }

    if (pConfig->topmost)
    {
        Topmost(hwnd);
    }

    else
    {
        SetWindowPos(hwnd, nullptr, pConfig->position[0], pConfig->position[1],
                     pConfig->position[2], pConfig->position[3], 0);
        ShowWindow(hwnd, SW_SHOWDEFAULT);
    }

    Uncloak(hwnd);
}

void MainWindow::Fullscreen(HWND hwnd)
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
                         mi.rcMonitor.bottom - mi.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }

    else
    {
        SetWindowLongPtrW(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        SetWindowPos(hwnd, nullptr, position.left, position.top, (position.right - position.left),
                     (position.bottom - position.top), 0);
    }
}

void MainWindow::Topmost(HWND hwnd)
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

bool MainWindow::SetDarkMode(HWND hwnd)
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

bool MainWindow::SetMica(HWND hwnd)
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

bool MainWindow::Cloak(HWND hwnd)
{
    auto cloak = TRUE;

    if (FAILED(DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloak, sizeof(cloak))))
        return false;

    return true;
}

bool MainWindow::Uncloak(HWND hwnd)
{
    auto uncloak = FALSE;

    if (FAILED(DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &uncloak, sizeof(uncloak))))
        return false;

    return true;
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
            return pMainWindow->_OnCommand();
        case WM_CREATE:
            return pMainWindow->_OnCreate(hwnd);
        case WM_ACTIVATE:
            return pMainWindow->_OnActivate(hwnd, wparam);
        case WM_CLOSE:
            return pMainWindow->_OnClose(hwnd);
        case WM_DESTROY:
            return pMainWindow->_OnDestroy();
        case WM_DPICHANGED:
            return pMainWindow->_OnDpiChanged();
        case WM_GETMINMAXINFO:
            return pMainWindow->_OnGetMinMaxInfo(hwnd, lparam);
        case WM_PAINT:
            return pMainWindow->_OnPaint(hwnd);
        case WM_SIZE:
            return pMainWindow->_OnSize(hwnd, wparam);
        case WM_SIZING:
            return pMainWindow->_OnSizing(hwnd);
        case WM_ENTERSIZEMOVE:
            return pMainWindow->_OnEnterSizeMove(hwnd);
        case WM_EXITSIZEMOVE:
            return pMainWindow->_OnExitSizeMove(hwnd);
        case WM_MOVE:
            return pMainWindow->_OnMove(hwnd);
        case WM_MOVING:
            return pMainWindow->_OnMoving(hwnd);
        case WM_WINDOWPOSCHANGING:
            return pMainWindow->_OnWindowPosChanging(hwnd);
        case WM_WINDOWPOSCHANGED:
            return pMainWindow->_OnWindowPosChanged(hwnd);
        case WM_SETFOCUS:
            return pMainWindow->_OnSetFocus(hwnd);
        case WM_SETTINGCHANGE:
            return pMainWindow->_OnSettingChange(hwnd);
        case WM_KEYDOWN:
            return pMainWindow->_OnKeyDown(hwnd, wparam);
        case WM_CHAR:
            return pMainWindow->_OnChar(hwnd, wparam);
        }
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

int MainWindow::_OnActivate(HWND hwnd, WPARAM wparam)
{
#ifdef _DEBUG
    println("WM_ACTIVATE");
#endif
    pConfig->Save();

    return 0;
}

int MainWindow::_OnChar(HWND hwnd, WPARAM wparam)
{
#ifdef _DEBUG
    println("WM_CHAR");
#endif

    return 0;
}

int MainWindow::_OnClose(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_CLOSE");
#endif
    DestroyWindow(hwnd);

    return 0;
}

int MainWindow::_OnCommand()
{
#ifdef _DEBUG
    println("WM_COMMAND");
#endif

    return 0;
}

int MainWindow::_OnCreate(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_CREATE");
#endif

    return 0;
}

int MainWindow::_OnDestroy()
{
#ifdef _DEBUG
    println("WM_DESTROY");
#endif
    PostQuitMessage(0);

    return 0;
}

int MainWindow::_OnDpiChanged()
{
#ifdef _DEBUG
    println("WM_DPICHANGED");
#endif

    return 0;
}

int MainWindow::_OnEnterSizeMove(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_ENTERSIZEMOVE");
#endif

    return 0;
}

int MainWindow::_OnExitSizeMove(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_EXITSIZEMOVE");
#endif
    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    GetWindowPlacement(hwnd, &wp);
    if (!pConfig->fullscreen & (wp.showCmd != 3))
    {
        RECT rect{0, 0, 0, 0};
        GetWindowRect(hwnd, &rect);
        pConfig->position = RectToBounds(rect);
    }

    pConfig->Save();

    return 0;
}

int MainWindow::_OnGetMinMaxInfo(HWND hwnd, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_GETMINMAXINFO");
#endif
    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    GetWindowPlacement(hwnd, &wp);
    if (wp.showCmd != 3)
    {
        pConfig->maximized = false;
        pConfig->Save();
    }

    LPMINMAXINFO minmax = (LPMINMAXINFO)lparam;
    minmax->ptMinTrackSize.x = 300;
    minmax->ptMinTrackSize.y = 300;

    return 0;
}

int MainWindow::_OnKeyDown(HWND hwnd, WPARAM wparam)
{
    if (wparam == VK_F1)
    {
#ifdef _DEBUG
        println("F1");
#endif
        pConfig->split = Toggle(pConfig->split);
        WebView::UpdateBounds(hwnd);
        WebView::UpdateFocus();
        WebView::SetWindowTitle(hwnd);
        WebView::SetWindowIcon(hwnd);
        pConfig->Save();
    }

    if (wparam == VK_F2)
    {
#ifdef _DEBUG
        println("F2");
#endif
        pConfig->swapped = Toggle(pConfig->swapped);
        WebView::UpdateBounds(hwnd);
        WebView::UpdateFocus();
        WebView::SetWindowTitle(hwnd);
        WebView::SetWindowIcon(hwnd);
        pConfig->Save();
    }

    if (wparam == VK_F4)
    {
#ifdef _DEBUG
        println("F4");
#endif
        pConfig->menu = Toggle(pConfig->menu);
        WebView::UpdateBounds(hwnd);
        WebView::UpdateFocus();
        WebView::SetWindowTitle(hwnd);
        WebView::SetWindowIcon(hwnd);
        pConfig->Save();
    }

    if (wparam == VK_F6)
    {
#ifdef _DEBUG
        println("F6");
#endif
        if (!pConfig->fullscreen)
        {
            WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
            GetWindowPlacement(hwnd, &wp);
            if (wp.showCmd == 3)
            {
                ShowWindow(hwnd, SW_SHOWNORMAL);
                SetWindowPos(hwnd, nullptr, pConfig->position[0], pConfig->position[1],
                             pConfig->position[2], pConfig->position[3], 0);
            }

            else
                ShowWindow(hwnd, SW_MAXIMIZE);
        }
    }

    if (wparam == VK_F11)
    {
#ifdef _DEBUG
        println("F11");
#endif
        pConfig->fullscreen = Toggle(pConfig->fullscreen);
        MainWindow::Fullscreen(hwnd);
        WebView::UpdateBounds(hwnd);
        pConfig->Save();
    }

    if (wparam == VK_F9)
    {
#ifdef _DEBUG
        println("F9");
#endif
        pConfig->topmost = Toggle(pConfig->topmost);
        MainWindow::Topmost(hwnd);
        WebView::SetWindowTitle(hwnd);
        pConfig->Save();
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

int MainWindow::_OnMove(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_MOVE");
#endif

    return 0;
}

int MainWindow::_OnMoving(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_MOVING");
#endif

    return 0;
}

int MainWindow::_OnPaint(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_PAINT");
#endif
    PAINTSTRUCT ps{};
    RECT bounds{};
    HDC hdc = BeginPaint(hwnd, &ps);
    GetClientRect(hwnd, &bounds);
    FillRect(hdc, &bounds, (HBRUSH)GetStockObject(BLACK_BRUSH));
    EndPaint(hwnd, &ps);

    return 0;
}

int MainWindow::_OnSetFocus(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_SETFOCUS");
#endif
    WebView::UpdateFocus();

    return 0;
}

int MainWindow::_OnSettingChange(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_SETTINGCHANGE");
#endif
    InvalidateRect(hwnd, nullptr, true);
    SetDarkMode(hwnd);

    return 0;
}

int MainWindow::_OnSize(HWND hwnd, WPARAM wparam)
{
#ifdef _DEBUG
    println("WM_SIZE");
#endif
    if (wparam == 2)
    {
        pConfig->maximized = true;
        pConfig->Save();
    }

    return 0;
}

int MainWindow::_OnSizing(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_SIZING");
#endif

    return 0;
}

int MainWindow::_OnWindowPosChanged(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_WINDOWPOSCHANGED");
#endif
    WebView::UpdateBounds(hwnd);

    return 0;
}

int MainWindow::_OnWindowPosChanging(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_WINDOWPOSCHANGING");
#endif

    return 0;
}
