bool CheckSystemDarkMode()
{
    using namespace winrt::Windows::UI;
    using namespace winrt::Windows::UI::ViewManagement;
    UISettings settingsCheck = UISettings();
    Color fgCheck = settingsCheck.GetColorValue(UIColorType::Foreground);
    return (((5 * fgCheck.G) + (2 * fgCheck.R) + fgCheck.B) > (8 * 128));
}

bool SetDarkTitle()
{
    using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode appMode);
    auto uxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (uxtheme)
    {
        auto ord135 = GetProcAddress(uxtheme, PCSTR MAKEINTRESOURCEW(135));
        if (ord135)
        {
            auto SetPreferredAppMode = reinterpret_cast<fnSetPreferredAppMode>(ord135);
            SetPreferredAppMode(PreferredAppMode::AllowDark);
        }
        FreeLibrary(uxtheme);
        return true;
    }
    return false;
}

bool SetDarkMode(HWND window)
{
    auto dwmtrue = TRUE;
    auto dwmfalse = FALSE;

    if (systemDarkMode)
    {
        DwmSetWindowAttribute(window, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmtrue, sizeof(dwmtrue));
        return true;
    }
    if (!systemDarkMode)
    {
        DwmSetWindowAttribute(window, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmfalse, sizeof(dwmfalse));
    }
    return false;
}

bool SetMica(HWND window)
{
    MARGINS m = {0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};
    auto extend = S_OK;
    extend = DwmExtendFrameIntoClientArea(window, &m);
    if (SUCCEEDED(extend))
    {
        auto backdrop = S_OK;
        backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;
        backdrop =
            DwmSetWindowAttribute(window, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(&backdrop));
        if (SUCCEEDED(backdrop))
        {
            return true;
        }
        return false;
    }
    return false;
}

bool SetWindow(HWND window, int ncs)
{
    auto cloakOn = TRUE;
    auto cloakOff = FALSE;
    auto cloak = S_OK;
    cloak = DwmSetWindowAttribute(window, DWMWA_CLOAK, &cloakOn, sizeof(cloakOn));
    if (SUCCEEDED(cloak))
    {
        auto uncloak = S_OK;
        SetWindowPos(window, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
        uncloak = DwmSetWindowAttribute(window, DWMWA_CLOAK, &cloakOff, sizeof(cloakOff));
        if (SUCCEEDED(uncloak))
        {
            ShowWindow(window, ncs);
            return true;
        }
        return false;
    }
    return false;
}
