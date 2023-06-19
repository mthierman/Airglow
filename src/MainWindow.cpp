#include "MainWindow.hpp"

MainWindow::MainWindow(HINSTANCE hinstance, int ncs){};

std::unique_ptr<MainWindow> MainWindow::Create(HINSTANCE hinstance, int ncs)
{
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

    auto pMainWindow = std::unique_ptr<MainWindow>(new MainWindow(hinstance, ncs));

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
            return pMainWindow->_OnCreate();
        case WM_DESTROY:
            return pMainWindow->_OnDestroy();
        case WM_DPICHANGED:
            return pMainWindow->_OnDpiChanged();
        case WM_GETMINMAXINFO:
            return pMainWindow->_OnGetMinMaxInfo(lparam);
        case WM_PAINT:
            return pMainWindow->_OnPaint(hwnd);
        case WM_SIZE:
            return pMainWindow->_OnSize();
        case WM_WINDOWPOSCHANGED:
            return pMainWindow->_OnWindowPosChanged(hwnd);
        case WM_SETTINGCHANGE:
            return pMainWindow->_OnSettingChange(hwnd);
        case WM_KEYDOWN:
            return pMainWindow->_OnKeyDown(hwnd, wparam);
        }

        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
}

int MainWindow::_OnCommand() { return 0; }

int MainWindow::_OnCreate()
{
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    SetEnvironmentVariableW(std::wstring(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR").c_str(),
                            std::wstring(L"0").c_str());

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

int MainWindow::_OnSize() { return 0; }

int MainWindow::_OnWindowPosChanged(HWND hwnd) { return 0; }

int MainWindow::_OnSettingChange(HWND hwnd)
{
    InvalidateRect(hwnd, nullptr, true);
    return 0;
}

int MainWindow::_OnKeyDown(HWND hwnd, WPARAM wparam) { return 0; }
