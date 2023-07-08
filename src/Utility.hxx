#pragma once

using json = nlohmann::json;
using path = std::filesystem::path;
using string = std::string;
using wstring = std::wstring;
using ofstream = std::ofstream;
using ifstream = std::ifstream;
using stringstream = std::stringstream;

namespace Utility
{
wstring to_wide(string);
string to_string(wstring);
string bool_to_string(bool);
wstring bool_to_wide(bool);
void print(string);
void println(string);
void wprint(wstring);
void wprintln(wstring);
void msgbox(string);
void msgboxw(wstring);
void error(string in);
void errorw(wstring in);
path DataPath();
path ConfigPath();
path DbPath();
std::pair<wstring, wstring> CommandLine();
bool Toggle(bool);
std::vector<int> RectToBounds(RECT);
RECT BoundsToRect(std::vector<int>);
void Tests(HWND);
}; // namespace Utility
