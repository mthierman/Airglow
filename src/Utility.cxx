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

int error(string in)
{
    wstring error = to_wide(in + ". Error: " + std::to_string(GetLastError()));
    MessageBoxW(nullptr, error.c_str(), wstring(L"Airglow").c_str(), 0);

    return 0;
};

int errorw(wstring in)
{
    wstring error = in + L". Error: " + std::to_wstring(GetLastError());
    MessageBoxW(nullptr, error.c_str(), wstring(L"Airglow").c_str(), 0);

    return 0;
};

int dberror(string in)
{
    MessageBoxW(nullptr, to_wide(in).c_str(), wstring(L"Airglow").c_str(), 0);

    return 0;
};

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
    wchar_t* wpgmptr;
    _get_wpgmptr(&wpgmptr);

    path exe = wpgmptr;

    return std::filesystem::canonical(path(wpgmptr).remove_filename());
}

path path_gui()
{
    auto data = path_portable();
    if (!std::filesystem::exists(data))
        return path{};

    return (data.wstring() + path::preferred_separator + to_wide("gui"));
}

path path_settings()
{
    auto data = path_gui();
    if (!std::filesystem::exists(data))
        return path{};

    return (L"file:///" + data.wstring() + path::preferred_separator + L"index.html");
}

path path_bar()
{
    auto data = path_gui();
    if (!std::filesystem::exists(data))
        return path{};

    return (to_wide("file:///") + data.wstring() + path::preferred_separator + to_wide("bar") +
            path::preferred_separator + to_wide("index.html"));
}

path path_json()
{
    auto data = path_portable();
    if (!std::filesystem::exists(data))
        return path{};

    return (data.wstring() + path::preferred_separator + to_wide("Airglow.json"));
}

path path_db()
{
    auto data = path_portable();
    if (!std::filesystem::exists(data))
        return path{};

    return (data.wstring() + path::preferred_separator + to_wide("Database.sqlite"));
}

path path_js()
{
    auto data = path_portable();
    if (!std::filesystem::exists(data))
        return path{};

    return (data.wstring() + path::preferred_separator + to_wide("js"));
}

path path_inject()
{
    auto data = path_js();
    if (!std::filesystem::exists(data))
        return path{};

    return (data.wstring() + path::preferred_separator + to_wide("inject.js"));
}

string system_color(winrt::Windows::UI::ViewManagement::UIColorType colorType)
{
    using namespace winrt::Windows::UI;
    using namespace winrt::Windows::UI::ViewManagement;

    UISettings settings{UISettings()};
    Color accent{settings.GetColorValue(colorType)};

    auto formatted{
        std::format("#{:0>2x}{:0>2x}{:0>2x}{:0>2x}", accent.R, accent.G, accent.B, accent.A)};

    return formatted;
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

    static RECT pos;

    auto style = GetWindowLongPtrW(hwnd, GWL_STYLE);

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

wstring js_inject()
{
    stringstream buffer{};
    wstring script{};

    if (std::filesystem::exists(path_inject()))
    {
        ifstream f(path_inject());
        if (!std::filesystem::is_empty(path_inject()))
        {
            buffer << f.rdbuf();
            script = to_wide(buffer.str());
        }
        f.close();
    }

    return script;
}

wstring js_inject_script()
{
    return wstring{LR"(
        document.onreadystatechange = () => {
            if (document.readyState === "interactive") {
                let scheme = document.createElement("meta");
                scheme.setAttribute("name", "color-scheme");
                scheme.setAttribute("content", "light dark");
                document.getElementsByTagName("head")[0].appendChild(scheme);
                document.documentElement.style.setProperty(
                    "color-scheme",
                    "light dark"
                );
            }
        };
    )"};
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

RECT window_bounds(HWND hwnd)
{
    RECT bounds{0, 0, 0, 0};
    GetClientRect(hwnd, &bounds);

    return bounds;
}

// RECT left_panel(RECT bounds)
// {
//     return RECT{
//         bounds.left,
//         bounds.top,
//         bounds.right / 2,
//         bounds.bottom,
//     };
// }

// RECT right_panel(RECT bounds)
// {
//     return RECT{
//         bounds.right / 2,
//         bounds.top,
//         bounds.right,
//         bounds.bottom,
//     };
// }

// RECT top_panel(RECT bounds)
// {
//     return RECT{
//         bounds.left,
//         bounds.top,
//         bounds.right,
//         bounds.bottom / 2,
//     };
// }

// RECT bottom_panel(RECT bounds)
// {
//     return RECT{
//         bounds.left,
//         bounds.bottom / 2,
//         bounds.right,
//         bounds.bottom,
//     };
// }

RECT full_panel(RECT bounds)
{
    return RECT{
        bounds.left,
        bounds.top,
        bounds.right,
        (bounds.bottom - 50),
    };
}

RECT left_panel(RECT bounds)
{
    return RECT{
        bounds.left,
        bounds.top,
        (bounds.right / 2),
        (bounds.bottom - 50),
    };
}

RECT right_panel(RECT bounds)
{
    return RECT{
        (bounds.right / 2),
        bounds.top,
        bounds.right,
        (bounds.bottom - 50),
    };
}

RECT top_panel(RECT bounds)
{
    return RECT{
        bounds.left,
        bounds.top,
        bounds.right,
        ((bounds.bottom / 2) - 25),
    };
}

RECT bottom_panel(RECT bounds)
{
    return RECT{
        bounds.left,
        ((bounds.bottom / 2) - 25),
        bounds.right,
        (bounds.bottom - 50),
    };
}

RECT bar_panel(RECT bounds)
{
    return RECT{
        bounds.left,
        (bounds.bottom - 50),
        bounds.right,
        bounds.bottom,
    };
}
} // namespace Utility
