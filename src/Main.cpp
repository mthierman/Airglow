#include "Main.hpp"
#include "Messages.hpp"
#include "Window.hpp"
#include "State.hpp"
#include "WebView.hpp"
#include "Settings.hpp"
#include "Utility.hpp"
#include "Debug.hpp"

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    window = InitializeWindow(instance, ncs);
    appData = GetAppDataPath();
    settingsFile = GetSettingsFilePath(appData);

    if (std::filesystem::exists(appData))
    {
        InitializeWebViews(window, appData);
    }

    Startup();

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

    // if (fullscreen)
    //     WindowFullscreen(window);
    // if (maximized)
    //     ShowWindow(window, SW_MAXIMIZE);
}

void Shutdown()
{
    // WINDOWPLACEMENT wp = {0};
    // wp.length = sizeof(WINDOWPLACEMENT);
    // GetWindowPlacement(window, &wp);
    // if (wp.showCmd == 3)
    //     maximized = true;

    // ShowWindow(window, SW_SHOWNORMAL);
    // RECT rect;
    // GetWindowRect(window, &rect);
    // dimensions = RectToBounds(rect);

    SaveSettings();
    Gdiplus::GdiplusShutdown(gdiplusToken);
    DestroyWindow(window);
}
