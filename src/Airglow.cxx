#include "MainWindow.hxx"
#include "Settings.hxx"
#include "WebView.hxx"

void Tests();

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    HWND hwnd = nullptr;

    auto window = MainWindow::Create(hinstance, ncs);

    if (!window)
        return 0;

    if (window)
        hwnd = window.get()->m_hWnd;

    if (hwnd)
        MainWindow::_ShowWindow(hwnd, ncs);

    auto settings = Settings::Create();
    auto appData = settings->GetAppDataPath();
    auto settingsFile = settings->GetSettingsFilePath();

    if (std::filesystem::exists(appData))
    {
        WebView::Create(hwnd, appData);
    }

    else
        return 0;

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
