#include "Main.hpp"
#include "Window.hpp"
#include "WndProc.hpp"
#include "Messages.hpp"
#include "WebView.hpp"
#include "Settings.hpp"
#include "Utility.hpp"

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    window = InitializeWindow(instance, ncs);
    Startup();

    appData = GetAppDataPath();
    settingsFile = GetSettingsFilePath(appData);

    if (std::filesystem::exists(appData))
    {
        InitializeWebViews(window, appData);
    }

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}

void Startup()
{
    LoadSettings();

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // SetWindowPos(window, nullptr, dimensions[0], dimensions[1], dimensions[2], dimensions[3], 0);
    if (maximized)
        ShowWindow(window, SW_MAXIMIZE);
}

void Shutdown()
{
    ShowWindow(window, SW_SHOWNORMAL);
    RECT rect;
    GetWindowRect(window, &rect);
    dimensions = RectToBounds(rect);

    SaveSettings();

    Gdiplus::GdiplusShutdown(gdiplusToken);

    DestroyWindow(window);
}
