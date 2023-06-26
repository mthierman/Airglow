#include "Utility.hxx"

namespace Utility
{

std::pair<wstring, wstring> CommandLine()
{
    std::pair<wstring, wstring> commands;

    auto cmd = GetCommandLineW();
    int number;

    auto args = CommandLineToArgvW(cmd, &number);

    if (number == 2)
    {
        commands.first = args[1];
        commands.second = args[1];
    }

    if (number == 3)
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

// std::pair<std::wstring, std::wstring> CommandLine()
// {
//     std::pair<std::wstring, std::wstring> pair;
//     int number;

//     auto cmd = GetCommandLineW();
//     auto args = CommandLineToArgvW(cmd, &number);

//     if (number == 2)
//     {
//         pair.first = args[1];
//         pair.second = args[1];
//     }

//     if (number == 3)
//     {
//         pair.first = args[1];
//         pair.second = args[2];
//     }

//     LocalFree(args);

//     return pair;
// }

bool Toggle(bool b) { return b ? false : true; }

position RectToBounds(RECT rect)
{
    position bounds = {rect.left, rect.top, (rect.right - rect.left), (rect.bottom - rect.top)};

    return bounds;
}

RECT BoundsToRect(position bounds)
{
    RECT rect = {bounds[0], bounds[1], (bounds[0] + bounds[2]), (bounds[1] + bounds[3])};

    return rect;
}

void print(string in) { OutputDebugStringW(ToWide(in).c_str()); }

void println(string in)
{
    OutputDebugStringW(ToWide(in).c_str());
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
    MessageBoxW(nullptr, ToWide(in).c_str(), wstring(L"Airglow").c_str(), 0);
};

void msgboxw(wstring in) { MessageBoxW(nullptr, in.c_str(), wstring(L"Airglow").c_str(), 0); };

void error(string in)
{
    wstring error = ToWide(in + ". Error: " + std::to_string(GetLastError()));
    MessageBoxW(nullptr, error.c_str(), wstring(L"Airglow").c_str(), 0);
};

void errorw(wstring in)
{
    wstring error = in + L". Error: " + std::to_wstring(GetLastError());
    MessageBoxW(nullptr, error.c_str(), wstring(L"Airglow").c_str(), 0);
};

wstring ToWide(string in)
{
    int size = MultiByteToWideChar(CP_UTF8, 0, in.c_str(), in.size(), nullptr, 0);
    wstring out;
    out.resize(size);
    MultiByteToWideChar(CP_UTF8, 0, in.c_str(), in.size(), out.data(), size);

    return out;
}

string ToString(wstring in)
{
    int size = WideCharToMultiByte(CP_UTF8, 0, in.c_str(), in.size(), nullptr, 0, nullptr, nullptr);
    string out;
    out.resize(size);
    WideCharToMultiByte(CP_UTF8, 0, in.c_str(), in.size(), out.data(), size, nullptr, nullptr);

    return out;
}

string BoolToString(bool input) { return input ? string("true") : string("false"); }

wstring BoolToWide(bool input) { return input ? wstring(L"true") : wstring(L"false"); }

void Tests(HWND hwnd)
{
    auto toggleTest = Toggle(false);
    if (toggleTest != false)
        print("Toggle(): TEST FAILED\n");

    wstring dpi = L"DPI: " + std::to_wstring(GetDpiForWindow(hwnd)) + L"\n";
    wprint(dpi);

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    wstring clientBounds = L"Client: " + std::to_wstring(clientRect.right - clientRect.left) +
                           L" x " + std::to_wstring(clientRect.bottom - clientRect.top) + L"\n";
    wprint(clientBounds.c_str());

    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);
    wstring windowBounds = L"Window: " + std::to_wstring(windowRect.right - windowRect.left) +
                           L" x " + std::to_wstring(windowRect.bottom - windowRect.top) + L"\n";
    wprint(windowBounds.c_str());

    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hwnd, &wp);
    if (wp.showCmd == 3)
        print("Window is maximized\n");

    auto acp = GetACP();
    wprint(std::to_wstring(acp));
}

path Utility::DataPath()
{
    path dataPath;
    wstring outBuffer;
    PWSTR buffer;

    if (SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &buffer) != S_OK)
    {
        CoTaskMemFree(buffer);
        return dataPath;
    }

    dataPath = wstring(buffer) + path::preferred_separator + L"Airglow";

    CoTaskMemFree(buffer);

    if (!std::filesystem::exists(dataPath))
        std::filesystem::create_directory(dataPath);

    return dataPath;
}

path Utility::SettingsPath(path dataPath)
{
    path settingsPath = (dataPath.wstring() + path::preferred_separator + L"Config.json");

    return settingsPath;
}
} // namespace Utility
