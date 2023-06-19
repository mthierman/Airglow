#include "MainWindow.hxx"

Settings* MainWindow::pSettings = nullptr;

unsigned long long MainWindow::gdiplusToken;
Gdiplus::GdiplusStartupInput MainWindow::gdiplusStartupInput;

MainWindow::MainWindow(HINSTANCE hinstance, int ncs, Settings* settings) {}

std::unique_ptr<MainWindow> MainWindow::Create(HINSTANCE hinstance, int ncs, Settings* settings)
{
    pSettings = settings;

    std::wstring className(L"airglow");
    std::wstring menuName(L"airglowmenu");
    std::wstring programIcon(L"PROGRAM_ICON");
    std::wstring appName(L"Airglow");

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
        std::wstring error =
            L"RegisterClassExW failed, last error is " + std::to_wstring(GetLastError());
        MessageBoxW(nullptr, error.c_str(), appName.c_str(), MB_ICONERROR);
        return nullptr;
    }

    auto pMainWindow = std::unique_ptr<MainWindow>(new MainWindow(hinstance, ncs, pSettings));

    HWND hwnd = CreateWindowExW(0, className.c_str(), appName.c_str(), WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                                nullptr, hinstance, pMainWindow.get());

    if (!hwnd)
    {
        std::wstring error =
            L"CreateWindowExW failed, last error is " + std::to_wstring(GetLastError());
        MessageBoxW(nullptr, error.c_str(), appName.c_str(), MB_ICONERROR);
        return nullptr;
    }

    return pMainWindow;
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
        case WM_CLOSE:
            return pMainWindow->_OnClose(hwnd);
        case WM_DESTROY:
            return pMainWindow->_OnDestroy();
        case WM_DPICHANGED:
            return pMainWindow->_OnDpiChanged();
        case WM_GETMINMAXINFO:
            return pMainWindow->_OnGetMinMaxInfo(lparam);
        case WM_PAINT:
            return pMainWindow->_OnPaint(hwnd);
        case WM_SIZE:
            return pMainWindow->_OnSize(hwnd);
        case WM_SIZING:
            return pMainWindow->_OnSizing(hwnd);
        case WM_WINDOWPOSCHANGED:
            return pMainWindow->_OnWindowPosChanged(hwnd);
        case WM_SETTINGCHANGE:
            return pMainWindow->_OnSettingChange(hwnd);
        case WM_KEYDOWN:
            return pMainWindow->_OnKeyDown(hwnd, wparam);
        }
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

int MainWindow::_OnCommand() { return 0; }

int MainWindow::_OnCreate(HWND hwnd)
{
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    SetEnvironmentVariableW(std::wstring(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR").c_str(),
                            std::wstring(L"0").c_str());

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    SetDarkTitle();
    SetDarkMode(hwnd);
    SetMica(hwnd);

    return 0;
}

int MainWindow::_OnClose(HWND hwnd)
{
    Gdiplus::GdiplusShutdown(gdiplusToken);
    DestroyWindow(hwnd);

    return 0;
}

int MainWindow::_OnDestroy()
{
    PostQuitMessage(0);

    return 0;
}

int MainWindow::_OnDpiChanged() { return 0; }

int MainWindow::_OnGetMinMaxInfo(LPARAM lparam)
{
    LPMINMAXINFO minmax = (LPMINMAXINFO)lparam;
    minmax->ptMinTrackSize.x = 800;
    minmax->ptMinTrackSize.y = 600;

    return 0;
}

int MainWindow::_OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    RECT bounds;
    GetClientRect(hwnd, &bounds);
    FillRect(hdc, &bounds, (HBRUSH)GetStockObject(BLACK_BRUSH));
    EndPaint(hwnd, &ps);

    return 0;
}

int MainWindow::_OnSize(HWND hwnd) { return 0; }

int MainWindow::_OnSizing(HWND hwnd) { return 0; }

int MainWindow::_OnWindowPosChanged(HWND hwnd)
{
    WebView::BoundsUpdate(hwnd);
    return 0;
}

int MainWindow::_OnSettingChange(HWND hwnd)
{
    InvalidateRect(hwnd, nullptr, true);
    return 0;
}

int MainWindow::_OnKeyDown(HWND hwnd, WPARAM wparam)
{
    if (wparam == VK_F1)
    {
        pSettings->boolSplit = Utility::Toggle(pSettings->boolSplit);
        WebView::BoundsUpdate(hwnd);
        WebView::FocusUpdate();
    }

    if (wparam == VK_F2)
    {
        pSettings->boolSwapped = Utility::Toggle(pSettings->boolSwapped);
        WebView::BoundsUpdate(hwnd);
        WebView::FocusUpdate();
    }

    if (wparam == VK_F4)
    {
        pSettings->boolMenu = Utility::Toggle(pSettings->boolMenu);
        WebView::BoundsUpdate(hwnd);
        WebView::FocusUpdate();
    }

    if (wparam == VK_F6)
    {
        if (!pSettings->boolFullscreen)
            pSettings->boolMaximized = Utility::Toggle(pSettings->boolMaximized);
        WebView::BoundsUpdate(hwnd);
        WebView::FocusUpdate();
    }

    if (wparam == VK_F11)
    {
        pSettings->boolFullscreen = Utility::Toggle(pSettings->boolFullscreen);
        WebView::BoundsUpdate(hwnd);
        WebView::FocusUpdate();
    }

    if (wparam == VK_F9)
    {
        pSettings->boolTopmost = Utility::Toggle(pSettings->boolTopmost);
        WebView::BoundsUpdate(hwnd);
        WebView::FocusUpdate();
    }

    if (wparam == 0x57)
    {
        auto state = GetKeyState(VK_CONTROL);
        if (state & 0x8000)
            SendMessageW(hwnd, WM_CLOSE, 0, 0);
    }

    return 0;
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

    if (uxtheme)
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

    return false;
}

bool MainWindow::SetDarkMode(HWND hwnd)
{
    auto dark = CheckSystemDarkMode();
    auto dwmtrue = TRUE;
    auto dwmfalse = FALSE;

    if (dark)
    {
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmtrue, sizeof(dwmtrue));

        return true;
    }

    if (!dark)
    {
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmfalse, sizeof(dwmfalse));

        return false;
    }

    return false;
}

bool MainWindow::SetMica(HWND hwnd)
{
    MARGINS m = {0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};
    auto extend = S_OK;

    extend = DwmExtendFrameIntoClientArea(hwnd, &m);

    if (SUCCEEDED(extend))
    {
        auto backdrop = S_OK;
        backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;

        backdrop =
            DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(&backdrop));

        if (SUCCEEDED(backdrop))
            return true;

        return false;
    }

    return false;
}

bool MainWindow::Show(HWND hwnd, int ncs)
{
    auto cloakOn = TRUE;
    auto cloakOff = FALSE;
    auto cloak = S_OK;

    cloak = DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloakOn, sizeof(cloakOn));

    if (SUCCEEDED(cloak))
    {
        auto uncloak = S_OK;

        SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
        uncloak = DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloakOff, sizeof(cloakOff));

        if (SUCCEEDED(uncloak))
        {
            ShowWindow(hwnd, ncs);

            return true;
        }

        return false;
    }

    return false;
}

void MainWindow::MaximizeWindow(HWND window)
{
    // if (!settings.fullscreen)
    // {
    //     if (!settings.maximized)
    //         ShowWindow(window, SW_NORMAL);
    //     else
    //         ShowWindow(window, SW_MAXIMIZE);
    // }
}

void MainWindow::FullscreenWindow(HWND hwnd)
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

void MainWindow::TopmostWindow(HWND hwnd)
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
