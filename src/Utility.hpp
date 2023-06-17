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
