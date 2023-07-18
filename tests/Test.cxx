#include "Config.hxx"

void Test(HWND hwnd)
{
    // println(get_system_color(winrt::Windows::UI::ViewManagement::UIColorType::Foreground));
    // println(get_system_color(winrt::Windows::UI::ViewManagement::UIColorType::Background));

    auto toggleTest = bool_toggle(false);
    if (toggleTest != false)
        println("Toggle(): TEST FAILED");

    wstring dpi = L"DPI: " + std::to_wstring(GetDpiForWindow(hwnd));
    wprintln(dpi);

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    wstring clientBounds = L"Client: " + std::to_wstring(clientRect.right - clientRect.left) +
                           L" x " + std::to_wstring(clientRect.bottom - clientRect.top);
    wprintln(clientBounds.c_str());

    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);
    wstring windowBounds = L"Window: " + std::to_wstring(windowRect.right - windowRect.left) +
                           L" x " + std::to_wstring(windowRect.bottom - windowRect.top);
    wprintln(windowBounds.c_str());

    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    GetWindowPlacement(hwnd, &wp);
    if (wp.showCmd == 3)
        println("Window is maximized");

    auto acp = GetACP();
    wprint(std::to_wstring(acp));
}

void App::Debug()
{
    println("theme: " + window.theme);
    println("mainUrl: " + window.mainUrl);
    println("sideUrl: " + window.sideUrl);
    println("menu: " + bool_to_string(window.menu));
    println("split: " + bool_to_string(window.split));
    println("swapped: " + bool_to_string(window.swapped));
    println("maximized: " + bool_to_string(window.maximized));
    println("fullscreen: " + bool_to_string(window.fullscreen));
    println("topmost: " + bool_to_string(window.topmost));
    println(std::to_string(window.position[0]));
    println(std::to_string(window.position[1]));
    println(std::to_string(window.position[2]));
    println(std::to_string(window.position[3]));
}
