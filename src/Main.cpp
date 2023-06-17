#include "Main.hpp"
#include "WndProc.hpp"
#include "App.hpp"
#include "Window.hpp"
#include "WebView.hpp"
#include "Settings.hpp"
#include "Utility.hpp"

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    auto window = InitializeWindow(instance, ncs);

    appData = GetAppDataPath();
    settingsFile = GetSettingsFilePath(appData);
    script = GetScript(appData);
    menuScript = GetMenuScript(appData);

    if (std::filesystem::exists(appData))
    {
        InitializeMenu(window, appData);
        InitializeMainPanel(window, appData);
        InitializeSidePanel(window, appData);
    }

    Startup(window);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
