// clang-format off
// ╔─────────────────────────────╗
// │                             │
// │     ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦     │  Airglow - https://github.com/mthierman/Airglow
// │     ╠═╣║╠╦╝║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                             │
// ╚─────────────────────────────╝
// clang-format on

#pragma once

// #include <GlowConfig.h>
#include <AirglowConfig.h>

#include <Windows.h>
#include <dwmapi.h>
#include <gdiplus.h>
#include <shcore.h>
#include <ShlObj.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.Graphics.Imaging.h>

#include <winrt/Microsoft.Web.WebView2.Core.h>

#include <nlohmann/json.hpp>
#include <sqlite3.h>
// #include <curl.h>

#include "Templates.hxx"
#include "Definitions.hxx"
