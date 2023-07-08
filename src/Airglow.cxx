#include "Airglow.hxx"
#include "Config.hxx"
#include "Database.hxx"
#include "MainWindow.hxx"
#include "WebView.hxx"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    auto pConfig{Config::Create()};
    if (!pConfig)
    {
        error("Configuration failed");
        return 0;
    }

    // auto pDatabase{Database::Create(pConfig.get())};
    // if (!pDatabase)
    // {
    //     error("Database creation failed");
    //     return 0;
    // }

    auto pWindow{MainWindow::Create(hinstance, ncs, pConfig.get())};
    if (!pWindow)
    {
        error("Window creation failed");
        return 0;
    }

    pWindow->Show(pWindow->m_hwnd, ncs);

    auto pWebView{WebView::Create(pWindow->m_hwnd, pConfig.get())};
    if (!pWebView)
    {
        error("WebView2 creation failed");
        return 0;
    }

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
