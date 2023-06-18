std::vector<int> RectToBounds(RECT rect)
{
    std::vector<int> bounds = {rect.left, rect.top, (rect.right - rect.left),
                               (rect.bottom - rect.top)};

    return bounds;
}

RECT BoundsToRect(std::vector<int> bounds)
{
    RECT rect = {bounds[0], bounds[1], (bounds[0] + bounds[2]), (bounds[1] + bounds[3])};

    return rect;
}

RECT GetMenuBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel = {0, 0, 0, 0};

    GetClientRect(window, &bounds);

    if (menu)
    {
        panel = {
            bounds.left,
            bounds.top,
            bounds.right,
            bounds.bottom,
        };
    }

    return panel;
}

RECT GetMainPanelBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel = {0, 0, 0, 0};

    GetClientRect(window, &bounds);

    if (menu)
        return panel;

    if (!split & !swapped)
        panel = bounds;

    if (!split & swapped)
        return panel;

    if (split & !swapped)
    {
        panel = {
            bounds.left,
            bounds.top,
            bounds.right / 2,
            bounds.bottom,
        };
    }

    if (split & swapped)
    {
        panel = {
            bounds.right / 2,
            bounds.top,
            bounds.right,
            bounds.bottom,
        };
    }

    return panel;
}

RECT GetSidePanelBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel = {0, 0, 0, 0};

    GetClientRect(window, &bounds);

    if (!split & !swapped)
        return panel;

    if (!split & swapped)
        panel = bounds;

    if (split & !swapped)
    {
        panel = {
            bounds.right / 2,
            bounds.top,
            bounds.right,
            bounds.bottom,
        };
    }

    if (split & swapped)
    {
        panel = {
            bounds.left,
            bounds.top,
            bounds.right / 2,
            bounds.bottom,
        };
    }

    return panel;
}

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
