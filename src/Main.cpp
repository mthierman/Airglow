#include "Main.hpp"
#include "WndProc.hpp"
#include "Window.hpp"
#include "WebView.hpp"
#include "Settings.hpp"

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    auto window = InitializeWindow(instance, ncs);

    auto appData = GetAppDataPath();

    if (std::filesystem::exists(appData))
    {
        InitializeMenu(window, appData);
        InitializeMainPanel(window, appData);
        InitializeSidePanel(window, appData);
    }

    auto settingsFile = GetSettingsFilePath();
    nlohmann::json settings = LoadSettings(settingsFile);
    fullscreen = settings["fullscreen"].get<bool>();
    maximized = settings["maximized"].get<bool>();
    menu = settings["menu"].get<bool>();
    ontop = settings["ontop"].get<bool>();
    split = settings["split"].get<bool>();
    swapped = settings["swapped"].get<bool>();
    if (fullscreen)
        WindowFullscreen(window);
    if (maximized)
        WindowMaximize(window);
    if (ontop)
        WindowTop(window);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
