class Settings
{
  public:
    static std::unique_ptr<Settings> Create();
    std::filesystem::path GetAppDataPath();
    std::filesystem::path GetSettingsFilePath();
    nlohmann::json DefaultSettings();
    nlohmann::json CurrentSettings();
    nlohmann::json LoadSettings();
    void SaveSettings();
    std::filesystem::path appData;
    std::filesystem::path settingsFile;

  private:
    Settings();
    std::vector<int> dimensions;
    bool menu;
    bool split;
    bool swapped;
    bool maximized;
    bool fullscreen;
    bool topmost;
    std::string mainpage;
    std::string sidepage;
};
