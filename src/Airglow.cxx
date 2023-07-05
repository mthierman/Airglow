#include "Airglow.hxx"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    unsigned long long gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    auto pConfig = Config::Create();

    if (!pConfig || !std::filesystem::exists(pConfig->pathData))
    {
        error(string("Data folder not found"));
        return 0;
    }

    auto pDatabase = Database::Create(pConfig.get());

    if (!pDatabase)
    {
        error(string("Database creation failed"));
        return 0;
    }

    auto pWindow = MainWindow::Create(hinstance, ncs, pConfig.get());
    auto hwnd = pWindow.get()->m_hWnd;

    if (!hwnd)
    {
        error(string("Window creation failed"));
        return 0;
    }

    pWindow->Show(hwnd, ncs);

    auto pWebView = WebView::Create(hwnd, pConfig.get());

    if (!pWebView)
    {
        error(string("WebView2 creation failed"));
        return 0;
    }

    MSG msg{};

    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    pConfig->Save();

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;
}
