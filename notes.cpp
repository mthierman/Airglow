WINDOWPLACEMENT wp = {};
wp.length = sizeof(WINDOWPLACEMENT);
auto placement = GetWindowPlacement(window, &wp);

RECT position;
GetWindowRect(window, &position);
