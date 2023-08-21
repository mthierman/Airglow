#include "Storage.hxx"

Storage::Storage()
{
    if (!std::filesystem::exists(paths.json))
        save();

    if (std::filesystem::exists(paths.json) && std::filesystem::is_empty(paths.json))
        save();

    if (std::filesystem::exists(paths.json))
    {
        auto j{load()};
        settings = deserialize_settings(j);
    }
}

Storage::~Storage() {}

nlohmann::json Storage::serialize()
{
    try
    {
        return nlohmann::json{{"settings",
                               {
                                   {"appName", settings.appName},
                                   {"appVersion", settings.appVersion},
                                   {"appTheme", settings.appTheme},
                                   {"appScale", settings.appScale},
                                   {"windowPosition", settings.windowPosition},
                                   {"windowMaximized", settings.windowMaximized},
                                   {"windowFullscreen", settings.windowFullscreen},
                                   {"windowTopmost", settings.windowTopmost},
                                   {"webviewGui", settings.webviewGui},
                                   {"webviewSplit", settings.webviewSplit},
                                   {"webviewSwapped", settings.webviewSwapped},
                                   {"webviewHorizontal", settings.webviewHorizontal},
                                   {"mainHomepage", settings.mainHomepage},
                                   {"mainCurrentPage", settings.mainCurrentPage},
                                   {"sideHomepage", settings.sideHomepage},
                                   {"sideCurrentPage", settings.sideCurrentPage},
                               }},
                              {"colors",
                               {{"accent", colors.accent},
                                {"accentDark1", colors.accentDark1},
                                {"accentDark2", colors.accentDark2},
                                {"accentDark3", colors.accentDark3},
                                {"accentLight1", colors.accentLight1},
                                {"accentLight2", colors.accentLight2},
                                {"accentLight3", colors.accentLight3},
                                {"background", colors.background},
                                {"foreground", colors.foreground}}}};
    }
    catch (const std::exception& e)
    {
        util::println(e.what());
        return nlohmann::json{};
    }
}

Settings Storage::deserialize_settings(nlohmann::json j)
{
    try
    {
        return Settings{j["settings"]["appName"].get<std::string>(),
                        j["settings"]["appVersion"].get<std::string>(),
                        j["settings"]["appTheme"].get<std::string>(),
                        j["settings"]["appScale"].get<float>(),
                        j["settings"]["windowPosition"].get<std::vector<int>>(),
                        j["settings"]["windowMaximized"].get<bool>(),
                        j["settings"]["windowFullscreen"].get<bool>(),
                        j["settings"]["windowTopmost"].get<bool>(),
                        j["settings"]["webviewGui"].get<bool>(),
                        j["settings"]["webviewSplit"].get<bool>(),
                        j["settings"]["webviewSwapped"].get<bool>(),
                        j["settings"]["webviewHorizontal"].get<bool>(),
                        j["settings"]["mainHomepage"].get<std::string>(),
                        j["settings"]["mainCurrentPage"].get<std::string>(),
                        j["settings"]["sideHomepage"].get<std::string>(),
                        j["settings"]["sideCurrentPage"].get<std::string>()};
    }
    catch (const std::exception& e)
    {
        util::println(e.what());
        return Settings{};
    }
}

Colors Storage::deserialize_colors(nlohmann::json j)
{
    try
    {
        return Colors{j["colors"]["accent"].get<std::string>(),
                      j["colors"]["accentDark1"].get<std::string>(),
                      j["colors"]["accentDark2"].get<std::string>(),
                      j["colors"]["accentDark3"].get<std::string>(),
                      j["colors"]["accentLight1"].get<std::string>(),
                      j["colors"]["accentLight2"].get<std::string>(),
                      j["colors"]["accentLight3"].get<std::string>(),
                      j["colors"]["background"].get<std::string>(),
                      j["colors"]["foreground"].get<std::string>()};
    }
    catch (const std::exception& e)
    {
        util::println(e.what());
        return Colors{};
    }
}

nlohmann::json Storage::load()
{
    try
    {
        std::ifstream f(util::path_json());
        nlohmann::json j{nlohmann::json::parse(f, nullptr, false, true)};
        f.close();

        return j;
    }
    catch (const std::exception& e)
    {
        util::println(e.what());
        return nlohmann::json{};
    }
}

void Storage::save()
{
    auto j{serialize()};
    try
    {
        std::ofstream f(util::path_json());
        f << std::setw(4) << j << "\n";
        f.close();
    }
    catch (const std::exception& e)
    {
        util::println(e.what());
        return;
    }
}
