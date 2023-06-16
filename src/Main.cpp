#include "Main.hpp"
#include "WndProc.hpp"
#include "Window.hpp"
#include "Theme.hpp"
#include "WebView.hpp"
#include "Settings.hpp"

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    auto window = InitializeWindow(instance, ncs);
    auto appData = GetAppDataPath();
    auto settingsFile = GetSettingsFilePath();
    nlohmann::json settings = LoadSettings(settingsFile);

    if (!appData.empty())
    {
        // InitializeMenu(window, appData);
        InitializeMainPanel(window, appData);
        InitializeSidePanel(window, appData);
    }

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
