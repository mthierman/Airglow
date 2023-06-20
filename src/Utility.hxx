#pragma once

class Utility
{
  public:
    static bool Toggle(bool);
    static std::vector<int> RectToBounds(RECT);
    static RECT BoundsToRect(std::vector<int>);
    static void print(std::string);
    static void printw(std::wstring);
    static void msgbox(std::string);
    static void msgboxw(std::wstring);
    static void error(std::string in);
    static void errorw(std::wstring in);
    static std::wstring ToWide(std::string);
    static std::string ToString(std::wstring);
    static std::string BoolToString(bool);
    static std::wstring BoolToWide(bool);
    static std::pair<std::wstring, std::wstring> CommandLine();
    static void Tests(HWND);
};
