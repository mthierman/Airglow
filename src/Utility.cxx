#include "Utility.hxx"

namespace util
{
string to_string(wstring in)
{
    int size = WideCharToMultiByte(CP_UTF8, 0, in.c_str(), in.size(), nullptr, 0, nullptr, nullptr);
    string out;
    out.resize(size);
    WideCharToMultiByte(CP_UTF8, 0, in.c_str(), in.size(), out.data(), size, nullptr, nullptr);

    return out;
}

wstring to_wide(string in)
{
    int size = MultiByteToWideChar(CP_UTF8, 0, in.c_str(), in.size(), nullptr, 0);
    wstring out;
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
} // namespace util
