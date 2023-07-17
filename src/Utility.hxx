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
bool window_maximize(HWND);
bool window_fullscreen(HWND);
bool window_topmost(HWND);

std::vector<int> window_position(HWND);

std::pair<wstring, wstring> command_line();

namespace State
{
struct Window
{
    std::wstring name{to_wide(APP_NAME)};
    std::wstring version{to_wide(APP_NAME)};
    HWND hwnd{};
    HBRUSH darkBrush{(HBRUSH)GetStockObject(BLACK_BRUSH)};
    HBRUSH lightBrush{(HBRUSH)GetStockObject(WHITE_BRUSH)};
    HCURSOR cursor{};
    HICON icon{};
    string theme{};
    std::vector<int> position{0, 0, 0, 0};
    bool menu{false};
    bool split{false};
    bool swapped{false};
    bool maximized{false};
    bool topmost{false};
    bool fullscreen{false};
    string mainUrl{"google.com"};
    string sideUrl{"google.com"};
};

struct Path
{
    path data{path_portable()};
    path settings{path_settings()};
    path json{path_json()};
    path db{path_db()};
    path js{path_js()};
};

struct Color
{
    string accent{};
    string accentDark1{};
    string accentDark2{};
    string accentDark3{};
    string accentLight1{};
    string accentLight2{};
    string accentLight3{};
    string Background{};
    string Foreground{};
};
} // namespace State
}; // namespace Utility
