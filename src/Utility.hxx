#pragma once

using json = nlohmann::json;
using path = std::filesystem::path;
using string = std::string;
using wstring = std::wstring;
using ofstream = std::ofstream;
using ifstream = std::ifstream;
using stringstream = std::stringstream;
using position = std::vector<int>;

namespace Utility
{
std::pair<wstring, wstring> CommandLine();
bool Toggle(bool);
std::vector<int> RectToBounds(RECT);
RECT BoundsToRect(std::vector<int>);
void print(string);
void println(string);
void wprint(wstring);
void wprintln(wstring);
void msgbox(string);
void msgboxw(wstring);
void error(string in);
void errorw(wstring in);
wstring ToWide(string);
string ToString(wstring);
string BoolToString(bool);
wstring BoolToWide(bool);
void Tests(HWND);
path DataPath();
path SettingsPath(path appData);
}; // namespace Utility
