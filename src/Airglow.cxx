#include "Airglow.hxx"
#include "Config.hxx"
#include "Database.hxx"
#include "MainWindow.hxx"
#include "WebView.hxx"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    SetEnvironmentVariableW(wstring(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR").c_str(),
                            wstring(L"0").c_str());

    static unsigned long long gdiplusToken;
    static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) != Gdiplus::Status::Ok)
        error("GDI+ initialization failed");

    auto pConfig{Config::Create()};
    if (!pConfig)
    {
        error("Configuration failed");
        return 0;
    }

    auto pDatabase{Database::Create(pConfig.get())};
    if (!pDatabase)
    {
        error("Database creation failed");
        return 0;
    }

    auto pMainWindow{MainWindow::Create(hinstance, ncs, pConfig.get())};
    if (!pMainWindow)
    {
        error("Window creation failed");
        return 0;
    }

    pMainWindow->Show();

    auto pWebView{WebView::Create(pConfig.get())};
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

    Gdiplus::GdiplusShutdown(gdiplusToken);

    pConfig->Save();

    return 0;
}
