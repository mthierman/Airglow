#pragma once

#define UNICODE
#define OEMRESOURCE

#include <windows.h>

#include <SDKDDKVer.h>

#include <dwmapi.h>
#include <shellapi.h>
#include <string>
#include <wingdi.h>

#include "wil/com.h"
#include <wrl.h>

#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.System.h"
#include "winrt/Windows.UI.Composition.h"
#include "winrt/Windows.UI.ViewManagement.h"
#include "winrt/base.h"

#include "Resource.h"
#include "WebView2/WebView2.h"

#pragma comment(lib, "dwmapi")
#pragma comment(lib, "gdi32")
