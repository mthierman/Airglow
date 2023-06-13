void CommandLineUrl()
{
    int nArgs;
    int i;
    LPWSTR commandLine = GetCommandLineW();
    LPWSTR* commandLineList = CommandLineToArgvW(commandLine, &nArgs);

    if (nArgs == 2)
    {
        url1 = commandLineList[1];
        url2 = commandLineList[1];
        isSplit = false;
    }
    if (nArgs == 3)
    {
        url1 = commandLineList[1];
        url2 = commandLineList[2];
        isSplit = true;
    }

    LocalFree(commandLineList);
}
