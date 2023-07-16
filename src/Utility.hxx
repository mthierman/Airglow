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

path path_appdata();
path path_portable();
path path_settings();
path path_json();
path path_db();
path path_js();

string system_color(winrt::Windows::UI::ViewManagement::UIColorType);
string system_theme();
string window_theme(HWND);

bool window_darktitle();

bool window_cloak(HWND);
bool window_uncloak(HWND);

bool window_mica(HWND);
void window_maximize(HWND);
void window_fullscreen(HWND);
void window_topmost(HWND);

std::vector<int> window_position(HWND);
}; // namespace Utility
