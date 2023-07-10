#pragma once

#include "Config.hxx"

class WebView
{
  public:
    static std::unique_ptr<WebView> Create(Config*);
    static void UpdateBounds();
    static void UpdateFocus();
    static void SetWindowTitle();
    static void SetWindowIcon();
    static void Fullscreen();
    static void Topmost();

  private:
    static Config* pConfig;
    static std::pair<wstring, wstring> CommandLine();
    static void Messages(wstring);
    static RECT FullBounds();
    static RECT MenuBounds();
    static RECT MainBounds();
    static RECT SideBounds();
    static wstring GetScriptFile();
    static wstring GetScript();
    static wstring GetMenuScript();

  protected:
    WebView(Config*);
};
