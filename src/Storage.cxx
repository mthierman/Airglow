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
                                   {"appTheme", settings.appTheme},
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
                               {{"colorAccent", colors.colorAccent},
                                {"colorAccentDark1", colors.colorAccentDark1},
                                {"colorAccentDark2", colors.colorAccentDark2},
                                {"colorAccentDark3", colors.colorAccentDark3},
                                {"colorAccentLight1", colors.colorAccentLight1},
                                {"colorAccentLight2", colors.colorAccentLight2},
                                {"colorAccentLight3", colors.colorAccentLight3},
                                {"colorBackground", colors.colorBackground},
                                {"colorForeground", colors.colorForeground}}}};
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
        return Settings{j["settings"]["appTheme"].get<std::string>(),
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
        return Colors{j["colors"]["colorAccent"].get<std::string>(),
                      j["colors"]["colorAccentDark1"].get<std::string>(),
                      j["colors"]["colorAccentDark2"].get<std::string>(),
                      j["colors"]["colorAccentDark3"].get<std::string>(),
                      j["colors"]["colorAccentLight1"].get<std::string>(),
                      j["colors"]["colorAccentLight2"].get<std::string>(),
                      j["colors"]["colorAccentLight3"].get<std::string>(),
                      j["colors"]["colorBackground"].get<std::string>(),
                      j["colors"]["colorForeground"].get<std::string>()};
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
