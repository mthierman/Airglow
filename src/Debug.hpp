// void Tests()
// {
//     std::println(stdout, "{}", std::string("Tests:"));

//     bool testBool = false;
//     auto test = Toggle(testBool);
//     OutputDebugStringW(BoolToWide(test).c_str());

//     OutputDebugStringW(BoolToWide(maximized).c_str());

//     WINDOWPLACEMENT wp = {};
//     wp.length = sizeof(WINDOWPLACEMENT);
//     GetWindowPlacement(window, &wp);
//     if (wp.showCmd == 3)
//         OutputDebugStringW(L"MAXIMIZED!");
// }

void DebugMessages()
{
    auto toggleTest = Toggle(false);
    if (toggleTest != false)
        std::println(stderr, "Toggle(): TEST FAILED");

    std::wstring dpi = L"DPI: " + std::to_wstring(GetDpiForWindow(window)) + L"\n";
    std::print(stderr, "{}", ToString(dpi));

    RECT clientRect;
    GetClientRect(window, &clientRect);
    std::wstring clientBounds = L"Client: " + std::to_wstring(clientRect.right - clientRect.left) +
                                L" x " + std::to_wstring(clientRect.bottom - clientRect.top) +
                                L"\n";
    std::print(stderr, "{}", ToString(clientBounds));

    RECT windowRect;
    GetWindowRect(window, &windowRect);
    std::wstring windowBounds = L"Window: " + std::to_wstring(windowRect.right - windowRect.left) +
                                L" x " + std::to_wstring(windowRect.bottom - windowRect.top) +
                                L"\n";
    std::print(stderr, "{}", ToString(windowBounds));

    // WINDOWPLACEMENT wp = {wp.length = sizeof(WINDOWPLACEMENT)};
    WINDOWPLACEMENT wp = {};
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(window, &wp);
    if (wp.showCmd == 3)
        std::println(stderr, "Maximized: true");
}
