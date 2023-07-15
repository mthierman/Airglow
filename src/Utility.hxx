#pragma once

namespace Utility
{
using ifstream = std::ifstream;
using json = nlohmann::json;
using ofstream = std::ofstream;
using path = std::filesystem::path;
using string = std::string;
using stringstream = std::stringstream;
using wstring = std::wstring;
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
string system_theme();
string system_color(winrt::Windows::UI::ViewManagement::UIColorType);
bool window_cloak(HWND);
bool window_uncloak(HWND);
bool window_allowdark();
bool window_darkmode(HWND);
bool window_mica(HWND);
bool window_maximize(HWND);
bool window_fullscreen(HWND);
bool window_topmost(HWND);
void window_show(HWND);
std::vector<int> window_position(HWND);
path path_appdata();
path path_portable();
path path_settings();
path path_json();
path path_db();
path path_js();
}; // namespace Utility
