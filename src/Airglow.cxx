#include "Airglow.hxx"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    HWND hwnd;
    std::unique_ptr<Config> pConfig;
    std::unique_ptr<MainWindow> pWindow;
    std::unique_ptr<WebView> pWebView;
    unsigned long long gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    pConfig = Config::Create();

    if (!std::filesystem::exists(pConfig->pathData))
    {
        error(string("Data folder not found"));
        return 0;
    }

    pWindow = MainWindow::Create(hinstance, ncs, pConfig.get());

    hwnd = pWindow.get()->m_hWnd;

    if (!hwnd)
    {
        error(string("Window creation failed"));
        return 0;
    }

    pWindow->Show(hwnd, ncs);

    pWebView = WebView::Create(hwnd, pConfig.get());

    WebView::Initialize(hwnd);

    auto db = Database::Create(pConfig.get());

#ifdef _DEBUG
#endif

    MSG msg = {};

    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    pConfig->Save();

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;
}
