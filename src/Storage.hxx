#pragma once

#include "Utility.hxx"

struct Application
{
    HCURSOR hCursor{
        (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED)};
    HICON hIcon{
        (HICON)LoadImageW(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE)};
    HBRUSH darkBrush{(HBRUSH)GetStockObject(BLACK_BRUSH)};
    HBRUSH lightBrush{(HBRUSH)GetStockObject(WHITE_BRUSH)};
    HBRUSH nullBrush{(HBRUSH)GetStockObject(NULL_BRUSH)};
};

struct Settings
{
    std::string appName{APP_NAME};
    std::string appVersion{APP_VERSION};
    std::string appTheme{util::system_theme()};
    float appScale{1};
    std::vector<int> windowPosition{0, 0, 0, 0};
    bool windowMaximized{false};
    bool windowFullscreen{false};
    bool windowTopmost{false};
    bool webviewGui{false};
    bool webviewSplit{false};
    bool webviewSwapped{false};
    bool webviewHorizontal{false};
    std::string mainHomepage{};
    std::string mainCurrentPage{};
    std::string sideHomepage{};
    std::string sideCurrentPage{};
};

struct Colors
{
    std::string accent{util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::Accent)};
    std::string accentDark1{
        util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1)};
    std::string accentDark2{
        util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2)};
    std::string accentDark3{
        util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3)};
    std::string accentLight1{
        util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1)};
    std::string accentLight2{
        util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2)};
    std::string accentLight3{
        util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3)};
    std::string background{
        util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::Background)};
    std::string foreground{
        util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::Foreground)};
};

struct Paths
{
    std::filesystem::path data{util::path_portable()};
    std::filesystem::path gui{util::path_gui()};
    std::filesystem::path json{util::path_json()};
    std::filesystem::path db{util::path_db()};
};

class Storage
{
  public:
    Storage();
    ~Storage();
    nlohmann::json serialize();
    Settings deserialize_settings(nlohmann::json j);
    Colors deserialize_colors(nlohmann::json j);
    nlohmann::json load();
    void save();

    Application application{};
    Settings settings{};
    Paths paths{};
    Colors colors{};
};
