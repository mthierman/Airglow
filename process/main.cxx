#include <Windows.h>

#include <filesystem>
#include <print>
#include <string>

auto get_pgmptr() -> std::filesystem::path
{
    std::string buffer;
    _get_pgmptr(std::out_ptr(buffer));
    std::filesystem::path exe{buffer};

    return std::filesystem::canonical(exe.remove_filename());
}

auto main() -> int
{
    std::println("Starting server...");

    STARTUPINFOA si{sizeof(STARTUPINFOA)};
    PROCESS_INFORMATION pi{};

    auto server{(get_pgmptr() / "server.exe").string()};
    auto pServer{server.data()};

    std::println("Server path: {}", server);

    CreateProcessA(pServer, nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
