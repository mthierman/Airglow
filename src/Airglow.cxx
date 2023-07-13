#include "Airglow.hxx"
#include "Config.hxx"
#include "Database.hxx"
#include "MainWindow.hxx"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    SetEnvironmentVariableW(to_wide("WEBVIEW2_DEFAULT_BACKGROUND_COLOR").c_str(),
                            to_wide("0").c_str());

    static unsigned long long gdiplusToken;
    static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) != Gdiplus::Status::Ok)
    {
        error("GDI+ initialization failed");
        return 0;
    }

    auto config{Config::Create()};
    if (!config)
    {
        error("Configuration failed");
        return 0;
    }

    auto database{Database::Create(config.get())};
    if (!database)
    {
        error("Database creation failed");
        return 0;
    }

    auto mainWindow{MainWindow::Create(hinstance, ncs, config.get())};
    if (!mainWindow)
    {
        error("Window creation failed");
        return 0;
    }

    config->PortableAppDataPath();

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    config->Save();

    return 0;
}
