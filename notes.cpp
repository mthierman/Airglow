WINDOWPLACEMENT wp = {};
wp.length = sizeof(WINDOWPLACEMENT);
GetWindowPlacement(window, &wp);

RECT position;
GetWindowRect(window, &position);
