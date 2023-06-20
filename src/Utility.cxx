#include "Utility.hxx"

bool Utility::Toggle(bool b) { return b ? false : true; }

std::vector<int> Utility::RectToBounds(RECT rect)
{
    std::vector<int> bounds = {rect.left, rect.top, (rect.right - rect.left),
                               (rect.bottom - rect.top)};

    return bounds;
}

RECT Utility::BoundsToRect(std::vector<int> bounds)
{
    RECT rect = {bounds[0], bounds[1], (bounds[0] + bounds[2]), (bounds[1] + bounds[3])};

    return rect;
}

void Utility::print(std::string in)
{
    OutputDebugStringW(ToWide(in).c_str());
    OutputDebugStringW(L"\n");
}

void Utility::printw(std::wstring in)
{
    OutputDebugStringW(in.c_str());
    OutputDebugStringW(L"\n");
}

void Utility::msgbox(std::string in)
{
    MessageBoxW(nullptr, ToWide(in).c_str(), std::wstring(L"Airglow").c_str(), 0);
};

void Utility::msgboxw(std::wstring in)
{
    MessageBoxW(nullptr, in.c_str(), std::wstring(L"Airglow").c_str(), 0);
};

void Utility::error(std::string in)
{
    std::wstring error = ToWide(in + ". Error: " + std::to_string(GetLastError()));
    MessageBoxW(nullptr, error.c_str(), std::wstring(L"Airglow").c_str(), 0);
};

void Utility::errorw(std::wstring in)
{
    std::wstring error = in + L". Error: " + std::to_wstring(GetLastError());
    MessageBoxW(nullptr, error.c_str(), std::wstring(L"Airglow").c_str(), 0);
};

std::wstring Utility::ToWide(std::string in)
{
    int size = MultiByteToWideChar(CP_UTF8, 0, in.c_str(), in.size(), nullptr, 0);
    std::wstring out;
    out.resize(size);
    MultiByteToWideChar(CP_UTF8, 0, in.c_str(), in.size(), out.data(), size);

    return out;
}

std::string Utility::ToString(std::wstring in)
{
    int size = WideCharToMultiByte(CP_UTF8, 0, in.c_str(), in.size(), nullptr, 0, nullptr, nullptr);
    std::string out;
    out.resize(size);
    WideCharToMultiByte(CP_UTF8, 0, in.c_str(), in.size(), out.data(), size, nullptr, nullptr);

    return out;
}

std::string Utility::BoolToString(bool input)
{
    return input ? std::string("true") : std::string("false");
}

std::wstring Utility::BoolToWide(bool input)
{
    return input ? std::wstring(L"true") : std::wstring(L"false");
}

std::pair<std::wstring, std::wstring> Utility::CommandLine()
{
    std::pair<std::wstring, std::wstring> pair;
    int number;

    auto cmd = GetCommandLineW();
    auto args = CommandLineToArgvW(cmd, &number);

    if (number == 2)
    {
        pair.first = args[1];
        pair.second = args[1];
    }

    if (number == 3)
    {
        pair.first = args[1];
        pair.second = args[2];
    }

    LocalFree(args);

    return pair;
}

void Utility::Tests(HWND hwnd)
{
    auto toggleTest = Utility::Toggle(false);
    if (toggleTest != false)
        Utility::print("Toggle(): TEST FAILED\n");

    std::wstring dpi = L"DPI: " + std::to_wstring(GetDpiForWindow(hwnd)) + L"\n";
    Utility::printw(dpi);

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    std::wstring clientBounds = L"Client: " + std::to_wstring(clientRect.right - clientRect.left) +
                                L" x " + std::to_wstring(clientRect.bottom - clientRect.top) +
                                L"\n";
    Utility::printw(clientBounds.c_str());

    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);
    std::wstring windowBounds = L"Window: " + std::to_wstring(windowRect.right - windowRect.left) +
                                L" x " + std::to_wstring(windowRect.bottom - windowRect.top) +
                                L"\n";
    Utility::printw(windowBounds.c_str());

    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hwnd, &wp);
    if (wp.showCmd == 3)
        Utility::print("Window is maximized\n");

    auto acp = GetACP();
    Utility::printw(std::to_wstring(acp));
}
