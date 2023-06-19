#pragma once

class Utility
{
  public:
    static std::vector<int> RectToBounds(RECT);
    static RECT BoundsToRect(std::vector<int>);
    static void prints(std::string);
    static void printw(std::wstring);
    static std::wstring ToWide(std::string);
    static std::string ToString(std::wstring);
    static std::wstring BoolToWide(bool);
};
