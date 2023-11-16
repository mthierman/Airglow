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

namespace util
{
std::string to_string(std::wstring);
std::wstring to_wstring(std::string);

void print(std::string);
void println(std::string);
void wprint(std::wstring);
void wprintln(std::wstring);
void msgbox(std::string);
void wmsgbox(std::wstring);
int error(std::string);
int werror(std::wstring);
int dberror(std::string);

unsigned long long startup();
void shutdown(unsigned long long);

std::filesystem::path path_appdata();
std::filesystem::path path_portable();
std::filesystem::path path_gui();
std::filesystem::path path_home();
std::filesystem::path path_settings();
std::filesystem::path path_bar();
std::filesystem::path path_json();
std::filesystem::path path_db();

std::string home_url();
std::string settings_url();
std::string bar_url();

std::pair<std::string, std::string> command_line();

FILE* create_console(bool);
void remove_console(FILE*);

std::string system_color(winrt::Windows::UI::ViewManagement::UIColorType);
std::string system_theme();

std::string window_theme(HWND);
bool window_darktitle();
bool window_cloak(HWND);
bool window_uncloak(HWND);
bool window_mica(HWND);
bool window_maximize(HWND);
bool window_fullscreen(HWND);
bool window_topmost(HWND);

RECT bounds(HWND);
std::vector<int> window_position(HWND);
} // namespace util
