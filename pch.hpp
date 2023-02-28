#pragma once

#define UNICODE
#define OEMRESOURCE

#include <windows.h>

#include <SDKDDKVer.h>

#include "Resource.h"

#include <dwmapi.h>
#include <shellapi.h>
#include <string>
#include <wingdi.h>

#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.System.h"
#include "winrt/Windows.UI.Composition.h"
#include "winrt/Windows.UI.ViewManagement.h"
#include "winrt/base.h"

#pragma comment(lib, "dwmapi")
#pragma comment(lib, "windowsapp")
// #pragma comment(lib, "gdi32")
