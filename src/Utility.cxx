#include "Utility.hxx"

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

void Utility::prints(std::string in)
{
    std::ostream_iterator<char> out(std::cout);
    std::format_to(out, "{}", in.c_str());
}

void Utility::printw(std::wstring in)
{
    std::ostream_iterator<char> out(std::cout);
    std::format_to(out, "{}", ToString(in).c_str());
}

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

std::wstring Utility::BoolToWide(bool input)
{
    return input ? std::wstring(L"true") : std::wstring(L"false");
}

