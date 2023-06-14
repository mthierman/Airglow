bool CommandLineUrl()
{
    int number;
    auto cmd = GetCommandLineW();
    auto args = CommandLineToArgvW(cmd, &number);
    if (number == 2)
    {
        url1 = args[1];
        url2 = args[1];
        isSplit = false;

        return true;
    }
    if (number == 3)
    {
        url1 = args[1];
        url2 = args[2];
        isSplit = true;

        return true;
    }
    LocalFree(args);

    return false;
}
