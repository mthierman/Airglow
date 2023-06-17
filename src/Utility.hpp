std::wstring ToWide(std::string input)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring output = converter.from_bytes(input);

    return output;
}

std::string ToString(std::wstring input)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string output = converter.to_bytes(input);

    return output;
}

std::wstring BoolToWide(bool input)
{
    return input ? std::wstring(L"true") : std::wstring(L"false");
}

std::pair<std::wstring, std::wstring> CommandLine()
{
    std::pair<std::wstring, std::wstring> pair;
    int number;

    auto cmd = GetCommandLineW();
    auto args = CommandLineToArgvW(cmd, &number);

    if (number == 2)
    {
        pair.first = args[1];
        pair.second = args[1];
    }

    if (number == 3)
    {
        pair.first = args[1];
        pair.second = args[2];
    }

    LocalFree(args);

    return pair;
}

void WebViewMessages(HWND window, PWSTR message)
{
    if ((std::wstring)message == std::wstring(L"F1").c_str())
    {
        PanelSplit(window);
    }

    if ((std::wstring)message == std::wstring(L"F2").c_str())
    {
        PanelSwap(window);
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if ((std::wstring)message == std::wstring(L"F4").c_str())
    {
        PanelHideMenu(window);
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if ((std::wstring)message == std::wstring(L"F5").c_str())
    {
        WindowMaximize(window);
    }

    if ((std::wstring)message == std::wstring(L"F11").c_str())
    {
        WindowFullscreen(window);
    }

    if ((std::wstring)message == std::wstring(L"F9").c_str())
    {
        WindowTop(window);
        SetWindowTitle(window);
        SetWindowIcon(window);
    }

    if ((std::wstring)message == std::wstring(L"close").c_str())
    {
        SendMessageW(window, WM_CLOSE, 0, 0);
    }
}
