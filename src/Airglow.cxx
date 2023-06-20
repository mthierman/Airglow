#include "Airglow.hxx"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    HWND hwnd;

    auto pSettings = Settings::Create();

    if (!std::filesystem::exists(pSettings->pathData))
    {
        std::wstring error =
            L"WebView data not found, last error is " + std::to_wstring(GetLastError());
        MessageBoxW(nullptr, error.c_str(), std::wstring(L"Airglow").c_str(), MB_ICONERROR);
        return 0;
    }

    auto pWindow = MainWindow::Create(hinstance, ncs, pSettings.get());

    if (!pWindow)
    {
        std::wstring error =
            L"Window creation failed, last error is " + std::to_wstring(GetLastError());
        MessageBoxW(nullptr, error.c_str(), std::wstring(L"Airglow").c_str(), MB_ICONERROR);
        return 0;
    }

    hwnd = pWindow.get()->m_hWnd;

    if (!hwnd)
    {
        std::wstring error =
            L"Window creation failed, last error is " + std::to_wstring(GetLastError());
        MessageBoxW(nullptr, error.c_str(), std::wstring(L"Airglow").c_str(), MB_ICONERROR);
        return 0;
    }

    pWindow->Show(hwnd, ncs);

    auto pWebView = WebView::Create(hwnd, pSettings.get());

    WebView::Initialize(hwnd);

#ifdef _DEBUG
    Utility::Tests(hwnd);
#endif

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
