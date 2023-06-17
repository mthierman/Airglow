std::filesystem::path GetAppDataPath()
{
    std::filesystem::path path;
    std::wstring outBuffer;
    PWSTR buffer;

    auto getKnownFolderPath = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &buffer);

    if (getKnownFolderPath != S_OK)
    {
        CoTaskMemFree(buffer);
        return path;
    }

    outBuffer = buffer;
    path = outBuffer + std::filesystem::path::preferred_separator + L"Airglow";

    CoTaskMemFree(buffer);

    if (!std::filesystem::exists(path))
        std::filesystem::create_directory(path);

    return path;
}

std::filesystem::path GetSettingsFilePath(std::filesystem::path appData)
{
    std::filesystem::path file =
        (appData.wstring() + std::filesystem::path::preferred_separator + L"Airglow.json");

    if (!std::filesystem::exists(file))
    {
        nlohmann::json defaultSettings = DefaultSettings();
        std::ofstream f(file);
        f << defaultSettings.dump(4);
        f.close();
    }

    return file;
}

std::wstring GetScriptFile(std::filesystem::path appData)
{
    std::wstring defaultScript = LR"(
        document.onreadystatechange = () => {
            if (document.readyState === "interactive") {
                let scheme = document.createElement("meta");
                scheme.setAttribute("name", "color-scheme");
                scheme.setAttribute("content", "light dark");
                document.getElementsByTagName("head")[0].appendChild(scheme);
                document.documentElement.style.setProperty(
                    "color-scheme",
                    "light dark"
                );
            }
            if (document.readyState === "complete") {
                onkeydown = (e) => {
                    if (e.key === "F3") {
                        e.preventDefault();
                    }
                    if (e.ctrlKey && e.key === "w") {
                        window.chrome.webview.postMessage("close");
                    } else {
                        window.chrome.webview.postMessage(e.key);
                    }
                };
            }
        };
    )";

    OutputDebugStringW(defaultScript.c_str());

    // std::stringstream buffer;
    // std::wstring script;

    // std::filesystem::path file =
    //     (appData.wstring() + std::filesystem::path::preferred_separator + L"Airglow.js");

    // if (!std::filesystem::exists(file))

    //     if (std::filesystem::exists(file))
    //     {
    //         std::ifstream f(file);
    //         if (!std::filesystem::is_empty(file))
    //         {
    //             buffer << f.rdbuf();
    //             script = ToWide(buffer.str());
    //         }
    //         f.close();
    //     }

    // return script;

    return defaultScript;
}

nlohmann::json DefaultSettings()
{
    nlohmann::json settings;
    settings["dimensions"] = {0, 0, 800, 600};
    settings["ontop"] = false;
    settings["maximized"] = false;
    settings["fullscreen"] = false;
    settings["split"] = false;
    settings["swapped"] = false;
    settings["menu"] = false;
    settings["mainpage"] = "https://wwww.google.com/";
    settings["sidepage"] = "https://wwww.google.com/";

    return settings;
}

nlohmann::json CurrentSettings(HWND window)
{
    nlohmann::json settings;
    settings["dimensions"] = GetBounds(window);
    settings["ontop"] = ontop;
    settings["maximized"] = maximized;
    settings["fullscreen"] = fullscreen;
    settings["split"] = split;
    settings["swapped"] = swapped;
    settings["menu"] = menu;
    settings["mainpage"] = ToString(mainpage);
    settings["sidepage"] = ToString(sidepage);

    return settings;
}

nlohmann::json LoadSettings(std::filesystem::path settingsFile)
{
    nlohmann::json saved = DefaultSettings();

    if (std::filesystem::exists(settingsFile))
    {
        std::ifstream f(settingsFile);
        if (!std::filesystem::is_empty(settingsFile))
            saved = nlohmann::json::parse(f);
        f.close();
    }

    return saved;
}

void SaveSettings(nlohmann::json input, std::filesystem::path settingsFile)
{
    if (std::filesystem::exists(settingsFile))
    {
        std::ofstream f(settingsFile);
        f << input.dump(4);
        f.close();
    }
}
