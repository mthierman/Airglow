#pragma once

namespace Utility
{
using json = nlohmann::json;
using path = std::filesystem::path;
using string = std::string;
using wstring = std::wstring;
using ofstream = std::ofstream;
using ifstream = std::ifstream;
using stringstream = std::stringstream;
string to_string(wstring);
wstring to_wide(string);
string bool_to_string(bool);
wstring bool_to_wide(bool);
bool bool_toggle(bool);
void print(string);
void println(string);
void wprint(wstring);
void wprintln(wstring);
void msgbox(string);
void msgboxw(wstring);
void error(string in);
void errorw(wstring in);
void dberror(string in);
RECT get_rect(HWND hwnd);
std::vector<int> rect_to_bounds(RECT);
RECT bounds_to_rect(std::vector<int>);
bool check_system_dark_mode();
string get_system_color(winrt::Windows::UI::ViewManagement::UIColorType);
}; // namespace Utility
