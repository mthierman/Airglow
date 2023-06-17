void Startup(HWND window)
{
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    auto settingsFile = GetSettingsFilePath(appData);

    nlohmann::json settings = LoadSettings(settingsFile);
    dimensions = settings["dimensions"].get<std::vector<int>>();
    fullscreen = settings["fullscreen"].get<bool>();
    maximized = settings["maximized"].get<bool>();
    menu = settings["menu"].get<bool>();
    ontop = settings["ontop"].get<bool>();
    split = settings["split"].get<bool>();
    swapped = settings["swapped"].get<bool>();
    mainpage = ToWide(settings["mainpage"].get<std::string>());
    sidepage = ToWide(settings["sidepage"].get<std::string>());

    // SetWindowPos(window, nullptr, dimensions[0], dimensions[1], dimensions[2], dimensions[3], 0);

    // if (fullscreen & maximized)
    // {
    //     fullscreen = false;
    //     maximized = false;
    // }

    // if (!fullscreen & maximized)
    //     WindowMaximize(window);

    // if (!maximized & fullscreen)
    //     WindowFullscreen(window);

    // if (ontop)
    //     WindowTop(window);
}

void Shutdown(HWND window)
{
    Gdiplus::GdiplusShutdown(gdiplusToken);

    auto settingsFile = GetSettingsFilePath(appData);
    nlohmann::json settings = CurrentSettings(window);
    SaveSettings(settings, settingsFile);

    DestroyWindow(window);
}
