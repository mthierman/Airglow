#include "Airglow.hxx"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    MSG msg{};

    auto pConfig = Config::Create();

    if (!pConfig || !std::filesystem::exists(pConfig->pathData))
    {
        error("Data folder not found");
        return 0;
    }

    auto pDatabase = Database::Create(pConfig.get());

    if (!pDatabase)
    {
        error("Database creation failed");
        return 0;
    }

    auto pWindow = MainWindow::Create(hinstance, ncs, pConfig.get());

    auto hwnd = pWindow->m_hwnd;

    pWindow->Show(hwnd, ncs);

    auto pWebView = WebView::Create(hwnd, pConfig.get());

    if (!pWebView)
    {
        error("WebView2 creation failed");
        return 0;
    }

    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
