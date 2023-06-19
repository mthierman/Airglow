#include "Airglow.hxx"
#include "MainWindow.hxx"
#include "WebView.hxx"
#include "Settings.hxx"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    HWND hwnd = nullptr;

    settings = Settings();

    auto window = MainWindow::Create(hinstance, ncs);

    if (!window)
    {
        std::wstring error =
            L"Window creation failed, last error is " + std::to_wstring(GetLastError());
        MessageBoxW(nullptr, error.c_str(), std::wstring(L"Airglow").c_str(), MB_ICONERROR);
        return 0;
    }

    hwnd = window.get()->m_hWnd;

    if (!hwnd)
    {
        std::wstring error =
            L"Window creation failed, last error is " + std::to_wstring(GetLastError());
        MessageBoxW(nullptr, error.c_str(), std::wstring(L"Airglow").c_str(), MB_ICONERROR);
        return 0;
    }

    MainWindow::_ShowWindow(hwnd, ncs);

    if (!std::filesystem::exists(settings.appData))
    {
        std::wstring error =
            L"WebView data not found, last error is " + std::to_wstring(GetLastError());
        MessageBoxW(nullptr, error.c_str(), std::wstring(L"Airglow").c_str(), MB_ICONERROR);
        return 0;
    }

    WebView::Create(hwnd, settings.appData);

    settings.LoadSettings();

#ifdef _DEBUG
    Tests();
#endif

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

void Tests() { OutputDebugStringW(L"Tests:\n"); }
