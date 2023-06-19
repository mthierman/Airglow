#pragma once

#include "MainWindow.hxx"
#include "Settings.hxx"
#include "Utility.hxx"

class WebView
{
  public:
    static std::unique_ptr<WebView> Create(HWND, Settings*);
    static void Initialize(HWND);
    static Settings* pSettings;
    static void Messages(std::wstring);
    static void UpdateBounds(HWND);
    static RECT GetFullBounds(HWND);
    static RECT GetMenuBounds(HWND);
    static RECT GetMainPanelBounds(HWND);
    static RECT GetSidePanelBounds(HWND);
    // void SetWindowTitle(HWND);
    // void SetWindowIcon(HWND);

  private:
    WebView(HWND, Settings*);
    static std::wstring GetScriptFile(std::filesystem::path);
    static std::wstring GetScript();
    static std::wstring GetMenuScript();
};
