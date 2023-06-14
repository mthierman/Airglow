unsigned short WindowClass(HINSTANCE instance)
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = instance;
    wcex.hCursor = (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = menuName.c_str();
    wcex.lpszClassName = className.c_str();
    wcex.hIcon = (HICON)LoadImageW(instance, programIcon.c_str(), IMAGE_ICON, 0, 0,
                                   LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
    wcex.hIconSm = (HICON)LoadImageW(instance, programIcon.c_str(), IMAGE_ICON, 0, 0,
                                     LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);

    return RegisterClassExW(&wcex);
}

HWND Window(HINSTANCE instance)
{
    return CreateWindowExW(0, className.c_str(), windowName.c_str(), WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                           nullptr, instance, nullptr);
}
