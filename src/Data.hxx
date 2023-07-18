#pragma once

#include "Utility.hxx"

using namespace Utility;

struct Window
{
    HWND hwnd;
    wstring name{to_wide(APP_NAME)};
    wstring version{to_wide(APP_NAME)};
    HBRUSH darkBrush{(HBRUSH)GetStockObject(BLACK_BRUSH)};
    HBRUSH lightBrush{(HBRUSH)GetStockObject(WHITE_BRUSH)};
    HCURSOR cursor{(HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED)};
    HICON icon{(HICON)LoadImageW(nullptr, (LPCWSTR)IDI_APPLICATION, IMAGE_ICON, 0, 0,
                                 LR_SHARED | LR_DEFAULTCOLOR | LR_DEFAULTSIZE)};
};

struct Settings
{
    string theme{"dark"};
    string mainUrl{"https://www.google.com/"};
    string sideUrl{"https://www.bing.com/"};
    std::vector<int> position{0, 0, 0, 0};
    bool menu{false};
    bool split{false};
    bool swapped{false};
    bool maximized{false};
    bool fullscreen{false};
    bool topmost{false};
};

struct Paths
{
    path data{path_portable()};
    path settings{path_settings()};
    path json{path_json()};
    path db{path_db()};
    path js{path_js()};
    path inject{path_inject()};
};

struct Colors
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
