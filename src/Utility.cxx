#include "Utility.hxx"

namespace util
{
std::string to_string(std::wstring in)
{
    if (!in.empty())
    {
        auto inSize{static_cast<int>(in.size())};

        auto outSize{WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS,
                                         in.data(), inSize, nullptr, 0, nullptr, nullptr)};

        if (outSize > 0)
        {
            std::string out;
            out.resize(static_cast<size_t>(outSize));

            if (WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_ERR_INVALID_CHARS, in.data(),
                                    inSize, out.data(), outSize, nullptr, nullptr) > 0)
                return out;
        }
    }

    return {};
}

std::wstring to_wide(std::string in)
{

    if (!in.empty())
    {
        auto inSize{static_cast<int>(in.size())};

        auto outSize{
            MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, in.data(), inSize, nullptr, 0)};

        if (outSize > 0)
        {
            std::wstring out;
            out.resize(static_cast<size_t>(outSize));

            if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, in.data(), inSize, out.data(),
                                    outSize) > 0)
                return out;
        }
    }

    return {};
}

std::string bool_to_string(bool in) { return in ? std::string("true") : std::string("false"); }

std::wstring bool_to_wide(bool in) { return in ? std::wstring(L"true") : std::wstring(L"false"); }

bool bool_toggle(bool b) { return b ? false : true; }

void print(std::string in) { OutputDebugStringW(to_wide(in).c_str()); }

void println(std::string in)
{
    OutputDebugStringW(to_wide(in).c_str());
    OutputDebugStringW(L"\n");
}

void wprint(std::wstring in) { OutputDebugStringW(in.c_str()); }

void wprintln(std::wstring in)
{
    OutputDebugStringW(in.c_str());
    OutputDebugStringW(L"\n");
}

void msgbox(std::string in) { MessageBoxW(nullptr, to_wide(in).c_str(), L"Message", 0); };

void wmsgbox(std::wstring in) { MessageBoxW(nullptr, in.c_str(), L"Message", 0); };

int error(std::string in)
{
    std::wstring error = to_wide(in + ". Error: " + std::to_string(GetLastError()));
    MessageBoxW(nullptr, error.c_str(), L"Error", 0);

    return 0;
};

int werror(std::wstring in)
{
    std::wstring error = in + L". Error: " + std::to_wstring(GetLastError());
    MessageBoxW(nullptr, error.c_str(), L"Error", 0);

    return 0;
};

int dberror(std::string in)
{
    MessageBoxW(nullptr, to_wide(in).c_str(), L"Error", 0);

    return 0;
};

std::filesystem::path path_appdata()
{
    PWSTR buffer{};

    if (FAILED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &buffer)))
        return std::filesystem::path{};

    std::filesystem::path data =
        std::wstring(buffer) + std::filesystem::path::preferred_separator + to_wide("Airglow");

    CoTaskMemFree(buffer);

    if (!std::filesystem::exists(data))
        std::filesystem::create_directory(data);

    return data;
}

std::filesystem::path path_portable()
{
    wchar_t* wpgmptr;
    _get_wpgmptr(&wpgmptr);

    std::filesystem::path exe = wpgmptr;

    return std::filesystem::canonical(exe.remove_filename());
}

std::filesystem::path path_gui()
{
    auto data = path_portable();
    if (!std::filesystem::exists(data))
        return std::filesystem::path{};

    return (data.wstring() + std::filesystem::path::preferred_separator + to_wide("gui"));
}

std::filesystem::path path_home()
{
    auto data = path_gui();
    if (!std::filesystem::exists(data))
        return std::filesystem::path{};

    return (L"file:///" + data.wstring() + std::filesystem::path::preferred_separator +
            to_wide("index.html"));
}

std::filesystem::path path_settings()
{
    auto data = path_gui();
    if (!std::filesystem::exists(data))
        return std::filesystem::path{};

    return (L"file:///" + data.wstring() + std::filesystem::path::preferred_separator +
            to_wide("settings") + std::filesystem::path::preferred_separator +
            to_wide("index.html"));
}

std::filesystem::path path_bar()
{
    auto data = path_gui();
    if (!std::filesystem::exists(data))
        return std::filesystem::path{};

    return (L"file:///" + data.wstring() + std::filesystem::path::preferred_separator +
            to_wide("bar") + std::filesystem::path::preferred_separator + to_wide("index.html"));
}

std::filesystem::path path_json()
{
    auto data = path_portable();
    if (!std::filesystem::exists(data))
        return std::filesystem::path{};

    return (data.wstring() + std::filesystem::path::preferred_separator + to_wide("Airglow.json"));
}

std::filesystem::path path_db()
{
    auto data = path_portable();
    if (!std::filesystem::exists(data))
        return std::filesystem::path{};

    return (data.wstring() + std::filesystem::path::preferred_separator +
            to_wide("Database.sqlite"));
}

winrt::hstring home_url()
{
#ifdef _DEBUG
    // return winrt::hstring(L"https://localhost:8000/");
    return winrt::to_hstring(path_home().c_str());
#endif
}

winrt::hstring settings_url()
{
#ifdef _DEBUG
    // return winrt::hstring(L"https://localhost:8000/settings/");
    return winrt::to_hstring(path_settings().c_str());
#endif
}

winrt::hstring bar_url()
{
#ifdef _DEBUG
    // return winrt::hstring(L"https://localhost:8000/bar/");
    return winrt::to_hstring(path_bar().c_str());
#else
    // return winrt::to_hstring(path_bar());
#endif
}

std::pair<winrt::hstring, winrt::hstring> command_line()
{
    std::pair<winrt::hstring, winrt::hstring> commands;

    auto cmd{GetCommandLineW()};
    int count;

    auto args{CommandLineToArgvW(cmd, &count)};

    if (count == 2)
    {
        commands.first = args[1];
        commands.second = winrt::hstring{};
    }

    if (count == 3)
    {
        commands.first = args[1];
        commands.second = args[2];
    }

    LocalFree(args);

    if (!commands.first.empty())
    {
        if (!commands.first.starts_with(L"http") || !commands.first.starts_with(L"https"))
            commands.first = L"https://" + commands.first;
    }

    if (!commands.second.empty())
    {
        if (!commands.second.starts_with(L"http") || !commands.second.starts_with(L"https"))
            commands.second = L"https://" + commands.second;
    }

    return commands;
}

std::pair<HWND, FILE*> create_console(HWND hwnd)
{
    FILE* dummyFile{};
    AllocConsole();
    hwnd = GetConsoleWindow();
    SetConsoleTitleW(L"Debug");
    window_mica(hwnd);
    SetWindowPos(hwnd, nullptr, 0, 0, 400, 400, SWP_SHOWWINDOW);
    // window_topmost(hwnd);
    freopen_s(&dummyFile, "CONOUT$", "w", stdout);
    freopen_s(&dummyFile, "CONOUT$", "w", stderr);
    freopen_s(&dummyFile, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    return std::make_pair(hwnd, dummyFile);
}

void remove_console(std::pair<HWND, FILE*> console)
{
    fclose(console.second);
    FreeConsole();
}

std::string system_color(winrt::Windows::UI::ViewManagement::UIColorType colorType)
{
    auto settings{winrt::Windows::UI::ViewManagement::UISettings()};
    auto accent{settings.GetColorValue(colorType)};

    auto formatted{
        std::format("#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", accent.R, accent.G, accent.B, accent.A)};

    return formatted;
}

std::string system_theme()
{
    auto settings{winrt::Windows::UI::ViewManagement::UISettings()};
    auto fg{settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground)};

    if (((5 * fg.G) + (2 * fg.R) + fg.B) > (8 * 128))
        return "dark";

    return "light";
}

std::string window_theme(HWND hwnd)
{
    auto dark{TRUE};
    auto light{FALSE};

    if (system_theme() == "light")
    {
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &light, sizeof(light));

        return "light";
    }

    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));

    return "dark";
}

bool window_darktitle()
{
    enum PreferredAppMode
    {
        Default,
        AllowDark,
        ForceDark,
        ForceLight,
        Max
    };

    using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode appMode);

    auto uxtheme{LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32)};

    if (!uxtheme)
        return false;

    auto ord135{GetProcAddress(uxtheme, PCSTR MAKEINTRESOURCEW(135))};

    if (!ord135)
        return false;

    auto SetPreferredAppMode{reinterpret_cast<fnSetPreferredAppMode>(ord135)};
    SetPreferredAppMode(PreferredAppMode::AllowDark);
    FreeLibrary(uxtheme);

    return true;
}

bool window_cloak(HWND hwnd)
{
    auto cloak{TRUE};

    if (FAILED(DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &cloak, sizeof(cloak))))
        return false;

    return true;
}

bool window_uncloak(HWND hwnd)
{
    auto uncloak{FALSE};

    if (FAILED(DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &uncloak, sizeof(uncloak))))
        return false;

    return true;
}

bool window_mica(HWND hwnd)
{
    MARGINS m{0, 0, 0, GetSystemMetrics(SM_CYVIRTUALSCREEN)};
    auto backdrop{DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW};

    if (FAILED(DwmExtendFrameIntoClientArea(hwnd, &m)))
        return false;

    if (FAILED(
            DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(&backdrop))))
        return false;

    return true;
}

bool window_maximize(HWND hwnd)
{
    auto style{GetWindowLongPtrW(hwnd, GWL_STYLE)};

    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    GetWindowPlacement(hwnd, &wp);

    if ((style & WS_OVERLAPPEDWINDOW) && wp.showCmd == 3)
    {
        ShowWindow(hwnd, SW_SHOWNORMAL);

        return false;
    }

    else
    {
        ShowWindow(hwnd, SW_MAXIMIZE);

        return true;
    }
}

bool window_fullscreen(HWND hwnd)
{
    window_cloak(hwnd);

    static RECT pos;

    auto style{GetWindowLongPtrW(hwnd, GWL_STYLE)};

    if (style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi = {sizeof(mi)};
        GetWindowRect(hwnd, &pos);
        if (GetMonitorInfoW(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi))
        {
            SetWindowLongPtrW(hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED);
        }
        window_uncloak(hwnd);

        return true;
    }

    else
    {
        SetWindowLongPtrW(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(hwnd, nullptr, pos.left, pos.top, (pos.right - pos.left),
                     (pos.bottom - pos.top), SWP_FRAMECHANGED);
        SendMessage(hwnd, WM_SETICON, 0, 0);
        window_uncloak(hwnd);

        return false;
    }
}

bool window_topmost(HWND hwnd)
{
    FLASHWINFO fwi{sizeof(FLASHWINFO)};
    fwi.hwnd = hwnd;
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 100;

    auto style{GetWindowLongPtrW(hwnd, GWL_EXSTYLE)};

    if (style & WS_EX_TOPMOST)
    {
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);

        return false;
    }

    else
    {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);

        return true;
    }
}

std::vector<int> window_position(HWND hwnd)
{
    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    RECT rect{0, 0, 0, 0};
    GetWindowPlacement(hwnd, &wp);
    GetWindowRect(hwnd, &rect);
    return bounds_to_position(rect);
}

std::vector<int> bounds_to_position(RECT rect)
{
    return std::vector<int>{rect.left, rect.top, (rect.right - rect.left),
                            (rect.bottom - rect.top)};
}

RECT bounds(HWND hwnd)
{
    RECT bounds{0, 0, 0, 0};
    GetClientRect(hwnd, &bounds);

    return bounds;
}

// RECT position_to_rect(std::vector<int> bounds)
// {
//     return RECT{bounds[0], bounds[1], (bounds[0] + bounds[2]), (bounds[1] + bounds[3])};
// }

// winrt::Windows::Foundation::Rect winrt_bounds_to_rect(RECT bounds)
// {
//     return winrt::Windows::Foundation::Rect{static_cast<float>(bounds.left),
//                                             static_cast<float>(bounds.top),
//                                             static_cast<float>(bounds.right - bounds.left),
//                                             static_cast<float>(bounds.bottom - bounds.top)};
// }

// RECT winrt_rect(winrt::Windows::Foundation::Rect rect)
// {
//     return RECT{static_cast<long>(rect.X), static_cast<long>(rect.Y),
//     static_cast<long>(rect.Width),
//                 static_cast<long>(rect.Height)};
// }
} // namespace util
