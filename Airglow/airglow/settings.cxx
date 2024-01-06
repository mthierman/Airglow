// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/settings.hxx>

namespace airglow::settings
{

// std::filesystem::path path_appdata()
// {
//     PWSTR buffer{};

//     if (FAILED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &buffer)))
//         return std::filesystem::path{};

//     std::filesystem::path data =
//         std::wstring(buffer) + std::filesystem::path::preferred_separator +
//         to_wstring("Airglow");

//     CoTaskMemFree(buffer);

//     if (!std::filesystem::exists(data)) std::filesystem::create_directory(data);

//     return data;
// }

// std::filesystem::path path_portable()
// {
//     wchar_t* wpgmptr;
//     _get_wpgmptr(&wpgmptr);

//     std::filesystem::path exe = wpgmptr;

//     return std::filesystem::canonical(exe.remove_filename());
// }

// std::filesystem::path path_gui()
// {
//     auto data = path_portable();
//     if (!std::filesystem::exists(data)) return std::filesystem::path{};

//     return (data.wstring() + std::filesystem::path::preferred_separator + to_wstring("gui"));
// }

// std::filesystem::path path_home()
// {
//     auto data = path_gui();
//     if (!std::filesystem::exists(data)) return std::filesystem::path{};

//     return (L"file:///" + data.wstring() + std::filesystem::path::preferred_separator +
//             to_wstring("index.html"));
// }

// std::filesystem::path path_settings()
// {
//     auto data = path_gui();
//     if (!std::filesystem::exists(data)) return std::filesystem::path{};

//     return (L"file:///" + data.wstring() + std::filesystem::path::preferred_separator +
//             to_wstring("settings") + std::filesystem::path::preferred_separator +
//             to_wstring("index.html"));
// }

// std::filesystem::path path_bar()
// {
//     auto data = path_gui();
//     if (!std::filesystem::exists(data)) return std::filesystem::path{};

//     return (L"file:///" + data.wstring() + std::filesystem::path::preferred_separator +
//             to_wstring("bar") + std::filesystem::path::preferred_separator +
//             to_wstring("index.html"));
// }

// std::filesystem::path path_json()
// {
//     auto data = path_portable();
//     if (!std::filesystem::exists(data)) return std::filesystem::path{};

//     return (data.wstring() + std::filesystem::path::preferred_separator +
//             to_wstring("Airglow.json"));
// }

// std::filesystem::path path_db()
// {
//     auto data = path_portable();
//     if (!std::filesystem::exists(data)) return std::filesystem::path{};

//     return (data.wstring() + std::filesystem::path::preferred_separator +
//             to_wstring("Database.sqlite"));
// }

// std::string home_url()
// {
// #ifdef _DEBUG
//     return "https://localhost:8000/";
// #else
//     return path_home().string();
// #endif
// }

// std::string settings_url()
// {
// #ifdef _DEBUG
//     return "https://localhost:8000/settings/";
// #else
//     return path_settings().string();
// #endif
// }

// std::string bar_url()
// {
// #ifdef _DEBUG
//     return "https://localhost:8000/bar/";
// #else
//     return path_bar().string();
// #endif
// }

} // namespace airglow::settings
