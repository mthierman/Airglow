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
void error(string);
void errorw(wstring);
void dberror(string);

RECT get_rect(HWND);
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
    HWND hwnd{nullptr};
    wstring name{to_wide(APP_NAME)};
    wstring version{to_wide(APP_NAME)};
    HBRUSH darkBrush{(HBRUSH)GetStockObject(BLACK_BRUSH)};
    HBRUSH lightBrush{(HBRUSH)GetStockObject(WHITE_BRUSH)};
    HCURSOR cursor{(HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED)};
    HICON icon{(HICON)LoadImageW(nullptr, (LPCWSTR)IDI_APPLICATION, IMAGE_ICON, 0, 0,
                                 LR_SHARED | LR_DEFAULTCOLOR | LR_DEFAULTSIZE)};

    string theme{"light"};
    string mainUrl{"google.com"};
    string sideUrl{"google.com"};
    std::vector<int> position{0, 0, 0, 0};
    bool menu{false};
    bool split{false};
    bool swapped{false};
    bool maximized{false};
    bool fullscreen{false};
    bool topmost{false};
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

json window_serialize(Window);
Window window_deserialize(json);
json window_load_state(Path);
void window_save_state(Path, json);
} // namespace State
}; // namespace Utility
