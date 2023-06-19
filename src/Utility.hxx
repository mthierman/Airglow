#pragma once

class Utility
{
  public:
    static std::vector<int> RectToBounds(RECT);
    static RECT BoundsToRect(std::vector<int>);
    static RECT GetFullBounds(HWND);
    static RECT GetMenuBounds(HWND);
    static RECT GetMainPanelBounds(HWND);
    static RECT GetSidePanelBounds(HWND);
    static void prints(std::string);
    static void printw(std::wstring);
    static std::wstring ToWide(std::string);
    static std::string ToString(std::wstring);
    static std::wstring BoolToWide(bool);
    static std::pair<std::wstring, std::wstring> CommandLine();
};
