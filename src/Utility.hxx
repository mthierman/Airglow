#pragma once

namespace Utility
{
bool Toggle(bool);
std::vector<int> RectToBounds(RECT);
RECT BoundsToRect(std::vector<int>);
void print(std::string);
void printw(std::wstring);
void msgbox(std::string);
void msgboxw(std::wstring);
void error(std::string in);
void errorw(std::wstring in);
std::wstring ToWide(std::string);
std::string ToString(std::wstring);
std::string BoolToString(bool);
std::wstring BoolToWide(bool);
std::pair<std::wstring, std::wstring> CommandLine();
void Tests(HWND);
}; // namespace Utility