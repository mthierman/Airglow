#include "Utility.hxx"

namespace Utility
{
string to_string(wstring in)
{
    int size = WideCharToMultiByte(CP_UTF8, 0, in.c_str(), in.size(), nullptr, 0, nullptr, nullptr);
    string out{};
    out.resize(size);
    WideCharToMultiByte(CP_UTF8, 0, in.c_str(), in.size(), out.data(), size, nullptr, nullptr);

    return out;
}

wstring to_wide(string in)
{
    int size = MultiByteToWideChar(CP_UTF8, 0, in.c_str(), in.size(), nullptr, 0);
    wstring out{};
    out.resize(size);
    MultiByteToWideChar(CP_UTF8, 0, in.c_str(), in.size(), out.data(), size);

    return out;
}

string bool_to_string(bool in) { return in ? string("true") : string("false"); }

wstring bool_to_wide(bool in) { return in ? wstring(L"true") : wstring(L"false"); }

bool bool_toggle(bool b) { return b ? false : true; }

void print(string in) { OutputDebugStringW(to_wide(in).c_str()); }

void println(string in)
{
    OutputDebugStringW(to_wide(in).c_str());
    OutputDebugStringW(L"\n");
}

void wprint(wstring in) { OutputDebugStringW(in.c_str()); }

void wprintln(wstring in)
{
    OutputDebugStringW(in.c_str());
    OutputDebugStringW(L"\n");
}

void msgbox(string in)
{
    MessageBoxW(nullptr, to_wide(in).c_str(), wstring(L"Airglow").c_str(), 0);
};

void msgboxw(wstring in) { MessageBoxW(nullptr, in.c_str(), wstring(L"Airglow").c_str(), 0); };

void error(string in)
{
    wstring error = to_wide(in + ". Error: " + std::to_string(GetLastError()));
    MessageBoxW(nullptr, error.c_str(), wstring(L"Airglow").c_str(), 0);
};

void errorw(wstring in)
{
    wstring error = in + L". Error: " + std::to_wstring(GetLastError());
    MessageBoxW(nullptr, error.c_str(), wstring(L"Airglow").c_str(), 0);
};

void dberror(string in)
{
    MessageBoxW(nullptr, to_wide(in).c_str(), wstring(L"Airglow").c_str(), 0);
};

RECT get_rect(HWND hwnd)
{
    RECT bounds{0, 0, 0, 0};
    GetClientRect(hwnd, &bounds);

    return bounds;
}

std::vector<int> rect_to_bounds(RECT rect)
{
    return std::vector<int>{rect.left, rect.top, (rect.right - rect.left),
                            (rect.bottom - rect.top)};
}

RECT bounds_to_rect(std::vector<int> bounds)
{
    return RECT{bounds[0], bounds[1], (bounds[0] + bounds[2]), (bounds[1] + bounds[3])};
}

path path_appdata()
{
    PWSTR buffer{};

    if (FAILED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &buffer)))
        return path{};

    path data = wstring(buffer) + path::preferred_separator + to_wide("Airglow");

    CoTaskMemFree(buffer);

    if (!std::filesystem::exists(data))
        std::filesystem::create_directory(data);

    return data;
}

path path_portable()
{
    auto cmd = GetCommandLineW();
    int count;
    auto args = CommandLineToArgvW(cmd, &count);
    path data{args[0]};
    LocalFree(args);

    return std::filesystem::canonical(data.remove_filename());
}

path path_settings()
{
    auto data = path_portable();
    if (!std::filesystem::exists(data))
        return path{};

    return (data.wstring() + path::preferred_separator + to_wide("settings"));
}

path path_json()
{
    auto data = path_portable();
    if (!std::filesystem::exists(data))
        return path{};

    return (data.wstring() + path::preferred_separator + to_wide("airglow.json"));
}

path path_db()
{
    auto data = path_portable();
    if (!std::filesystem::exists(data))
        return path{};

    return (data.wstring() + path::preferred_separator + to_wide("airglow.sqlite"));
}

path path_js()
{
    auto data = path_portable();
    if (!std::filesystem::exists(data))
        return path{};

    return (data.wstring() + path::preferred_separator + to_wide("js"));
}

string system_color(winrt::Windows::UI::ViewManagement::UIColorType colorType)
{
    using namespace winrt::Windows::UI;
    using namespace winrt::Windows::UI::ViewManagement;

    UISettings settings{UISettings()};
    Color accent{settings.GetColorValue(colorType)};

    // std::stringstream version, old implementation:
    //
    // std::stringstream buffer;
    // buffer << "#";
    // buffer << std::hex << std::setfill('0');
    // buffer << std::hex << std::setw(2) << static_cast<int>(accent.R);
    // buffer << std::hex << std::setw(2) << static_cast<int>(accent.G);
    // buffer << std::hex << std::setw(2) << static_cast<int>(accent.B);
    // buffer << std::hex << std::setw(2) << static_cast<int>(accent.A);
    // return buffer.str();

    // new std::format version:
    //
    auto formatted{
        std::format("#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", accent.R, accent.G, accent.B, accent.A)};

    return formatted;
}

Colors system_colors()
{
    Colors system;
    system.accent = system_color(winrt::Windows::UI::ViewManagement::UIColorType::Accent);
    system.accentDark1 = system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1);
    system.accentDark2 = system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2);
    system.accentDark3 = system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3);
    system.accentLight1 =
        system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1);
    system.accentLight2 =
        system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2);
    system.accentLight3 =
        system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3);
    system.Background = system_color(winrt::Windows::UI::ViewManagement::UIColorType::Background);
    system.Foreground = system_color(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);

    return system;
}

string system_theme()
{
    using namespace winrt::Windows::UI;
    using namespace winrt::Windows::UI::ViewManagement;

    UISettings settings{UISettings()};
    Color fg{settings.GetColorValue(UIColorType::Foreground)};

    if (((5 * fg.G) + (2 * fg.R) + fg.B) > (8 * 128))
        return "dark";

    return "light";
}

string window_theme(HWND hwnd)
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
    auto backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;

    if (FAILED(DwmExtendFrameIntoClientArea(hwnd, &m)))
        return false;

    if (FAILED(
            DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(&backdrop))))
        return false;

    return true;
}

bool window_maximize(HWND hwnd)
{
    auto style = GetWindowLongPtrW(hwnd, GWL_STYLE);

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

    static RECT position;

    auto style = GetWindowLongPtrW(hwnd, GWL_STYLE);

    if (style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi = {sizeof(mi)};
        GetWindowRect(hwnd, &position);
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
        SetWindowPos(hwnd, nullptr, position.left, position.top, (position.right - position.left),
                     (position.bottom - position.top), SWP_FRAMECHANGED);
        SendMessage(hwnd, WM_SETICON, 0, 0);
        window_uncloak(hwnd);

        return false;
    }
}

bool window_topmost(HWND hwnd)
{
    FLASHWINFO fwi{};
    fwi.cbSize = sizeof(FLASHWINFO);
    fwi.hwnd = hwnd;
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 100;

    auto style = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);

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
    return rect_to_bounds(rect);
}

std::pair<wstring, wstring> command_line()
{
    std::pair<wstring, wstring> commands;

    auto cmd = GetCommandLineW();
    int count;

    auto args = CommandLineToArgvW(cmd, &count);

    if (count == 2)
    {
        commands.first = args[1];
        commands.second = wstring{};
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
} // namespace Utility
