// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "global.hxx"

Keys::Keys()
{
    set.emplace(VK_PAUSE);
    set.emplace(0x30);
    set.emplace(0x4C);
    set.emplace(0x57);
    set.emplace(VK_F1);
    set.emplace(VK_F2);
    set.emplace(VK_F3);
    set.emplace(VK_F4);
    // set.emplace(VK_F5);
    set.emplace(VK_F6);
    set.emplace(VK_F7);
    set.emplace(VK_F8);
    set.emplace(VK_F9);
    set.emplace(VK_F10);
    set.emplace(VK_F11);
    // set.emplace(VK_F12);
}

URL::URL() : home{homepage(), homepage()}, current{"about:blank", "about:blank"} {}

auto URL::homepage() -> std::string
{
#if defined(_DEBUG)
    return "https://localhost:8000/home/index.html";
#else
    return "file:///" + glow::app_path().generic_string() + "/gui/home/index.html";
#endif
}

Layout::Layout() : bar{}, border{2}, horizontal{}, split{}, swapped{} {}

Dimensions::Dimensions() : devicePixelRatio{}, offsetHeight{}, offsetWidth{} {}

auto log(std::string string) -> void
{
    OutputDebugStringA(string.c_str());
    OutputDebugStringA("\n");
}
