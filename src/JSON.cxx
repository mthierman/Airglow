#include "JSON.hxx"

JSON::JSON() {}

std::unique_ptr<JSON> JSON::Create()
{
    auto json{std::unique_ptr<JSON>(new JSON())};

    return json;
}

void JSON::Save()
{
    // try
    // {
    //     json j{};
    //     j["position"] = position;
    //     j["menu"] = menu;
    //     j["split"] = split;
    //     j["swapped"] = swapped;
    //     j["maximized"] = maximized;
    //     j["fullscreen"] = fullscreen;
    //     j["topmost"] = topmost;
    //     j["mainUrl"] = mainUrl;
    //     j["sideUrl"] = sideUrl;

    //     ofstream f(paths.config);
    //     f << std::setw(4) << j << "\n";
    //     f.close();
    // }
    // catch (const std::exception& e)
    // {
    //     return;
    // }
}

void JSON::Load()
{
    // if (std::filesystem::exists(paths.config) && !std::filesystem::is_empty(paths.config))
    // {
    //     try
    //     {
    //         json j{};
    //         ifstream f(paths.config);
    //         j = json::parse(f, nullptr, false, true);
    //         f.close();

    //         position = j["position"].get<std::vector<int>>();
    //         menu = j["menu"].get<bool>();
    //         split = j["split"].get<bool>();
    //         swapped = j["swapped"].get<bool>();
    //         maximized = j["maximized"].get<bool>();
    //         fullscreen = j["fullscreen"].get<bool>();
    //         topmost = j["topmost"].get<bool>();
    //         mainUrl = j["mainUrl"].get<string>();
    //         sideUrl = j["sideUrl"].get<string>();
    //     }
    //     catch (const std::exception& e)
    //     {
    //         return;
    //     }
    // }
}

// json App::GetJson()
// {
//     json j{{"settings",
//             {{"mainUrl", pConfig->settings.mainUrl},
//              {"sideUrl", pConfig->settings.sideUrl},
//              {"accentColor", pConfig->settings.accentColor}}}};
// }
