#pragma once

namespace util
{
std::string to_string(std::wstring);
std::wstring to_wide(std::string);
std::string bool_to_string(bool);
std::wstring bool_to_wide(bool);
bool bool_toggle(bool);
void print(std::string);
void println(std::string);
void wprint(std::wstring);
void wprintln(std::wstring);
void msgbox(std::string);
void wmsgbox(std::wstring);
int error(std::string);
int werror(std::wstring);
int dberror(std::string);

std::filesystem::path path_appdata();
std::filesystem::path path_portable();
std::filesystem::path path_gui();
std::filesystem::path path_home();
std::filesystem::path path_settings();
std::filesystem::path path_bar();
std::filesystem::path path_json();
std::filesystem::path path_db();

winrt::hstring home_url();
winrt::hstring settings_url();
winrt::hstring bar_url();

std::pair<winrt::hstring, winrt::hstring> command_line();

std::pair<HWND, FILE*> create_console(HWND);
void remove_console(std::pair<HWND, FILE*>);

std::string system_color(winrt::Windows::UI::ViewManagement::UIColorType);
std::string system_theme();

std::string window_theme(HWND);
bool window_darktitle();
bool window_cloak(HWND);
bool window_uncloak(HWND);
bool window_mica(HWND);
bool window_maximize(HWND);
bool window_fullscreen(HWND);
bool window_topmost(HWND);
std::vector<int> window_position(HWND);
std::vector<int> bounds_to_position(RECT);
RECT bounds(HWND);

// RECT bounds_to_rect(std::vector<int>);
// winrt::Windows::Foundation::Rect winrt_bounds_to_rect(RECT);
// RECT winrt_rect_to_bounds(winrt::Windows::Foundation::Rect);
} // namespace util
