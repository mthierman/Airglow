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
    string mainUrl{"google.com"};
    string sideUrl{"google.com"};
    std::vector<int> position{0, 0, 0, 0};
    bool menu{false};
    bool split{false};
    bool swapped{false};
    bool horizontal{false};
    bool maximized{false};
    bool fullscreen{false};
    bool topmost{false};

    json Serialize()
    {
        try
        {
            return json{{"settings",
                         {
                             {"theme", theme},
                             {"mainUrl", mainUrl},
                             {"sideUrl", sideUrl},
                             {"position", position},
                             {"menu", menu},
                             {"split", split},
                             {"swapped", swapped},
                             {"horizontal", horizontal},
                             {"maximized", maximized},
                             {"fullscreen", fullscreen},
                             {"topmost", topmost},
                         }}};
        }
        catch (const std::exception& e)
        {
            println(e.what());
            return json{};
        }
    }

    Settings Deserialize(json j)
    {
        try
        {
            return Settings{j["settings"]["theme"].get<string>(),
                            j["settings"]["mainUrl"].get<string>(),
                            j["settings"]["sideUrl"].get<string>(),
                            j["settings"]["position"].get<std::vector<int>>(),
                            j["settings"]["menu"].get<bool>(),
                            j["settings"]["split"].get<bool>(),
                            j["settings"]["swapped"].get<bool>(),
                            j["settings"]["horizontal"].get<bool>(),
                            j["settings"]["maximized"].get<bool>(),
                            j["settings"]["fullscreen"].get<bool>(),
                            j["settings"]["topmost"].get<bool>()

            };
        }
        catch (const std::exception& e)
        {
            println(e.what());
            return Settings{};
        }
    }
};

struct Paths
{
    path data{path_portable()};
    path gui{path_gui()};
    path json{path_json()};
    path db{path_db()};
    path js{path_js()};
    path inject{path_inject()};
};

struct Colors
{
    string accent{system_color(winrt::Windows::UI::ViewManagement::UIColorType::Accent)};
    string accentDark1{system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1)};
    string accentDark2{system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2)};
    string accentDark3{system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3)};
    string accentLight1{
        system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1)};
    string accentLight2{
        system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2)};
    string accentLight3{
        system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3)};
    string Background{system_color(winrt::Windows::UI::ViewManagement::UIColorType::Background)};
    string Foreground{system_color(winrt::Windows::UI::ViewManagement::UIColorType::Foreground)};

    json Serialize()
    {
        try
        {
            return json{{"colors",
                         {{"accent", accent},
                          {"accentDark1", accentDark1},
                          {"accentDark2", accentDark2},
                          {"accentDark3", accentDark3},
                          {"accentLight1", accentLight1},
                          {"accentLight2", accentLight2},
                          {"accentLight3", accentLight3},
                          {"background", Background},
                          {"foreground", Foreground}}}};
        }
        catch (const std::exception& e)
        {
            println(e.what());
            return json{};
        }
    }

    Colors Deserialize(json j)
    {
        try
        {
            return Colors{j["colors"]["accent"].get<string>(),
                          j["colors"]["accentDark1"].get<string>(),
                          j["colors"]["accentDark2"].get<string>(),
                          j["colors"]["accentDark3"].get<string>(),
                          j["colors"]["accentLight1"].get<string>(),
                          j["colors"]["accentLight2"].get<string>(),
                          j["colors"]["accentLight3"].get<string>(),
                          j["colors"]["background"].get<string>(),
                          j["colors"]["foreground"].get<string>()

            };
        }
        catch (const std::exception& e)
        {
            println(e.what());
            return Colors{};
        }
    }
};
