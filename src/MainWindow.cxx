#include "MainWindow.hxx"

Config* MainWindow::pConfig = nullptr;
unsigned long long MainWindow::gdiplusToken;
Gdiplus::GdiplusStartupInput MainWindow::gdiplusStartupInput;

MainWindow::MainWindow(HINSTANCE hinstance, int ncs, Config* config) {}

std::unique_ptr<MainWindow> MainWindow::Create(HINSTANCE hinstance, int ncs, Config* config)
{
    pConfig = config;

    wstring className(L"airglow");
    wstring menuName(L"airglowmenu");
    wstring programIcon(L"PROGRAM_ICON");
    wstring appName(L"Airglow");

    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.hInstance = hinstance;
    wcex.lpfnWndProc = MainWindow::_WndProc;
    wcex.lpszClassName = className.c_str();
    wcex.lpszMenuName = menuName.c_str();
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.hCursor = (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    wcex.hIcon = (HICON)LoadImageW(hinstance, programIcon.c_str(), IMAGE_ICON, 0, 0,
                                   LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
    wcex.hIconSm = (HICON)LoadImageW(hinstance, programIcon.c_str(), IMAGE_ICON, 0, 0,
                                     LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);

    if (RegisterClassExW(&wcex) == 0)
    {
        string err = "Register window failed";
        error(err);
        return 0;
    }

    auto pMainWindow = std::unique_ptr<MainWindow>(new MainWindow(hinstance, ncs, pConfig));

    HWND hwnd = CreateWindowExW(0, className.c_str(), appName.c_str(), WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                                nullptr, hinstance, pMainWindow.get());

    if (!hwnd)
    {
        string err = "Window creation failed";
        error(err);
        return 0;
    }

    return pMainWindow;
}

bool MainWindow::CheckSystemDarkMode()
{
    winrt::Windows::UI::ViewManagement::UISettings settingsCheck =
        winrt::Windows::UI::ViewManagement::UISettings();
    winrt::Windows::UI::Color fgCheck =
        settingsCheck.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);

    return (((5 * fgCheck.G) + (2 * fgCheck.R) + fgCheck.B) > (8 * 128));
}

bool MainWindow::SetDarkTitle()
{
    using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode appMode);

    auto uxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

    if (!uxtheme)
        return false;

    else
    {
        auto ord135 = GetProcAddress(uxtheme, PCSTR MAKEINTRESOURCEW(135));

        if (ord135)
        {
            auto SetPreferredAppMode = reinterpret_cast<fnSetPreferredAppMode>(ord135);
            SetPreferredAppMode(PreferredAppMode::AllowDark);
        }

        FreeLibrary(uxtheme);

        return true;
    }
}

bool MainWindow::SetDarkMode(HWND hwnd)
{
    auto dark = CheckSystemDarkMode();
    auto dwmtrue = TRUE;
    auto dwmfalse = FALSE;

    if (!dark)
    {
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmfalse, sizeof(dwmfalse));

        return false;
    }

    else
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmtrue, sizeof(dwmtrue));

    return true;
}

bool MainWindow::SetMica(HWND hwnd)
{
    MARGINS m = {0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};

    if (DwmExtendFrameIntoClientArea(hwnd, &m) != S_OK)
        return false;

    else
    {
        HRESULT backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;

        if (DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(&backdrop)) !=
            S_OK)
            return false;

        else
            return true;
    }
}

bool MainWindow::Cloak(HWND hwnd)
{
    auto cloakOn = TRUE;
    auto cloakOff = FALSE;

    if (DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloakOn, sizeof(cloakOn)) != S_OK)
        return false;

    else
        return true;
}

bool MainWindow::Uncloak(HWND hwnd)
{
    auto cloakOn = TRUE;
    auto cloakOff = FALSE;

    if (DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloakOff, sizeof(cloakOff)) != S_OK)
        return false;

    else
        return true;
}

void MainWindow::Show(HWND hwnd, int ncs)
{

    Cloak(hwnd);

    if (!pConfig->boolFullscreen & !pConfig->boolMaximized)
    {
        SetWindowPos(hwnd, nullptr, pConfig->vectorPosition[0], pConfig->vectorPosition[1],
                     pConfig->vectorPosition[2], pConfig->vectorPosition[3], 0);
        ShowWindow(hwnd, SW_SHOWNORMAL);
    }

    if (!pConfig->boolFullscreen & pConfig->boolMaximized)
        ShowWindow(hwnd, SW_MAXIMIZE);

    if (pConfig->boolFullscreen)
    {
        SetWindowPos(hwnd, nullptr, pConfig->vectorPosition[0], pConfig->vectorPosition[1],
                     pConfig->vectorPosition[2], pConfig->vectorPosition[3], 0);
        ShowWindow(hwnd, SW_SHOWNORMAL);
        Fullscreen(hwnd);
    }

    if (pConfig->boolTopmost)
    {
        Topmost(hwnd);
    }

    else
    {
        SetWindowPos(hwnd, nullptr, pConfig->vectorPosition[0], pConfig->vectorPosition[1],
                     pConfig->vectorPosition[2], pConfig->vectorPosition[3], 0);
        ShowWindow(hwnd, SW_SHOWDEFAULT);
    }

    Uncloak(hwnd);
}

void MainWindow::Fullscreen(HWND hwnd)
{
    Cloak(hwnd);

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

    Uncloak(hwnd);
}

void MainWindow::Topmost(HWND hwnd)
{
    FLASHWINFO fwi;
    fwi.cbSize = sizeof(FLASHWINFO);
    fwi.hwnd = hwnd;
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 100;
    auto top = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
    if (top & WS_EX_TOPMOST)
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

template <class T, class U, HWND(U::*m_hWnd)>
T* InstanceFromWndProc(HWND hwnd, UINT msg, LPARAM lparam)
{
    T* pInstance;

    if (msg == WM_NCCREATE)
    {
        LPCREATESTRUCTW pCreateStruct = reinterpret_cast<LPCREATESTRUCTW>(lparam);
        pInstance = reinterpret_cast<T*>(pCreateStruct->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pInstance));
        pInstance->*m_hWnd = hwnd;
    }

    else
    {
        pInstance = reinterpret_cast<T*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }

    return pInstance;
}

__int64 __stdcall MainWindow::_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    MainWindow* pMainWindow =
        InstanceFromWndProc<MainWindow, MainWindow, &MainWindow::m_hWnd>(hwnd, msg, lparam);

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
        // case WM_WINDOWPOSCHANGING:
        //     return pMainWindow->_OnWindowPosChanging(hwnd);
        // case WM_WINDOWPOSCHANGED:
        //     return pMainWindow->_OnWindowPosChanged(hwnd);
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
    pConfig->Tests();
#endif

    SetEnvironmentVariableW(wstring(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR").c_str(),
                            wstring(L"0").c_str());

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    SetDarkTitle();
    SetDarkMode(hwnd);
    SetMica(hwnd);

    return 0;
}

int MainWindow::_OnActivate(HWND hwnd, WPARAM wparam)
{
#ifdef _DEBUG
    println("WM_ACTIVATE");
#endif

    return 0;
}

int MainWindow::_OnClose(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_CLOSE");
#endif

    pConfig->Save();
    Gdiplus::GdiplusShutdown(gdiplusToken);
    DestroyWindow(hwnd);

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

int MainWindow::_OnGetMinMaxInfo(HWND hwnd, LPARAM lparam)
{
#ifdef _DEBUG
    println("WM_GETMINMAXINFO");
#endif

    WINDOWPLACEMENT wp = {sizeof(WINDOWPLACEMENT)};
    GetWindowPlacement(hwnd, &wp);
    if (wp.showCmd != 3)
    {
        pConfig->boolMaximized = false;
        pConfig->Save();
    }

    LPMINMAXINFO minmax = (LPMINMAXINFO)lparam;
    minmax->ptMinTrackSize.x = 300;
    minmax->ptMinTrackSize.y = 300;

    return 0;
}

int MainWindow::_OnPaint(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_PAINT");
#endif

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    RECT bounds;
    GetClientRect(hwnd, &bounds);
    FillRect(hdc, &bounds, (HBRUSH)GetStockObject(BLACK_BRUSH));
    EndPaint(hwnd, &ps);

    return 0;
}

int MainWindow::_OnSize(HWND hwnd, WPARAM wparam)
{
#ifdef _DEBUG
    println("WM_SIZE");
#endif

    if (wparam == 2)
    {
        pConfig->boolMaximized = true;
        pConfig->Save();
    }

    WebView::UpdateBounds(hwnd);

    return 0;
}

int MainWindow::_OnSizing(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_SIZING\n");
#endif

    return 0;
}

int MainWindow::_OnEnterSizeMove(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_ENTERSIZEMOVE\n");
#endif

    return 0;
}

int MainWindow::_OnExitSizeMove(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_EXITSIZEMOVE\n");
#endif

    WINDOWPLACEMENT wp = {sizeof(WINDOWPLACEMENT)};
    GetWindowPlacement(hwnd, &wp);

    if (!pConfig->boolFullscreen & (wp.showCmd != 3))
    {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        pConfig->vectorPosition = RectToBounds(rect);
    }

    pConfig->Save();

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

// int MainWindow::_OnWindowPosChanging(HWND hwnd)
// {
// #ifdef _DEBUG
//     println("WM_WINDOWPOSCHANGING");
// #endif

//     return 0;
// }

// int MainWindow::_OnWindowPosChanged(HWND hwnd)
// {
// #ifdef _DEBUG
//     println("WM_WINDOWPOSCHANGED");
// #endif

//     return 0;
// }

int MainWindow::_OnSetFocus(HWND hwnd)
{
#ifdef _DEBUG
    println("WM_SETFOCUS\n");
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

int MainWindow::_OnKeyDown(HWND hwnd, WPARAM wparam)
{
    if (wparam == VK_F1)
    {
#ifdef _DEBUG
        println("F1");
#endif

        pConfig->boolSplit = Toggle(pConfig->boolSplit);
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

        pConfig->boolSwapped = Toggle(pConfig->boolSwapped);
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

        pConfig->boolMenu = Toggle(pConfig->boolMenu);
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

        if (!pConfig->boolFullscreen)
        {
            WINDOWPLACEMENT wp = {sizeof(WINDOWPLACEMENT)};
            GetWindowPlacement(hwnd, &wp);
            if (wp.showCmd == 3)
            {
                ShowWindow(hwnd, SW_SHOWNORMAL);
                SetWindowPos(hwnd, nullptr, pConfig->vectorPosition[0], pConfig->vectorPosition[1],
                             pConfig->vectorPosition[2], pConfig->vectorPosition[3], 0);
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

        pConfig->boolFullscreen = Toggle(pConfig->boolFullscreen);
        MainWindow::Fullscreen(hwnd);
        WebView::UpdateBounds(hwnd);
        pConfig->Save();
    }

    if (wparam == VK_F9)
    {
#ifdef _DEBUG
        println("F9");
#endif

        pConfig->boolTopmost = Toggle(pConfig->boolTopmost);
        MainWindow::Topmost(hwnd);
        WebView::SetWindowTitle(hwnd);
        pConfig->Save();
    }

    if (wparam == 0x57)
    {
        auto state = GetKeyState(VK_CONTROL);
        if (state & 0x8000)
            PostMessageW(hwnd, WM_CLOSE, 0, 0);
    }

    return 0;
}

int MainWindow::_OnChar(HWND hwnd, WPARAM wparam)
{
#ifdef _DEBUG
    println("WM_CHAR");
#endif

    return 0;
}
