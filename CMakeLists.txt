cmake_minimum_required(VERSION 3.26)

set(APP_NAME "Airglow")
set(APP_VERSION "1.4.2")

set(WEBVIEW "Microsoft.Web.WebView2")
set(WEBVIEW_VER "1.0.2151.40")
set(CPPWINRT "Microsoft.Windows.CppWinRT")
set(CPPWINRT_VER "2.0.230706.1")
set(WIL "Microsoft.Windows.ImplementationLibrary")
set(WIL_VER "1.0.230824.2")

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>DLL)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${APP_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${APP_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${APP_NAME})
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/PDB)

project(
    ${APP_NAME}
    VERSION ${APP_VERSION}
    LANGUAGES C CXX ASM
)

include(FetchContent)

file(WRITE ${CMAKE_BINARY_DIR}/version.txt v${APP_VERSION})
execute_process(
    COMMAND git --no-pager log -5 --oneline --no-decorate
    OUTPUT_FILE ${CMAKE_BINARY_DIR}/release_notes.txt
)

configure_file(${CMAKE_SOURCE_DIR}/config/AirglowConfig.h.in ${CMAKE_BINARY_DIR}/config/AirglowConfig.h)
file(TO_CMAKE_PATH ${CMAKE_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.rc RC_FILE)
file(TO_CMAKE_PATH ${CMAKE_SOURCE_DIR}/data/app.manifest MANIFEST_FILE)

execute_process(
    COMMAND
    nuget install ${WEBVIEW} -Version ${WEBVIEW_VER} -OutputDirectory ${CMAKE_BINARY_DIR}/Nuget
)
file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/Nuget/${WEBVIEW}.${WEBVIEW_VER} webview_SOURCE_DIR)

execute_process(
    COMMAND
    nuget install ${CPPWINRT} -Version ${CPPWINRT_VER} -OutputDirectory ${CMAKE_BINARY_DIR}/Nuget
)
file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/Nuget/${CPPWINRT}.${CPPWINRT_VER} cppwinrt_SOURCE_DIR)

execute_process(
    COMMAND
    nuget install ${WIL} -Version ${WIL_VER} -OutputDirectory ${CMAKE_BINARY_DIR}/Nuget
)
file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/Nuget/${WIL}.${WIL_VER} wil_SOURCE_DIR)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    sqlite
    URL https://www.sqlite.org/2023/sqlite-amalgamation-3440000.zip
)
FetchContent_MakeAvailable(sqlite)

# FetchContent_Declare(
#     curl
#     URL https://github.com/curl/curl/releases/download/curl-8_4_0/curl-8.4.0.tar.xz
# )
# FetchContent_MakeAvailable(curl)

execute_process(
    COMMAND cppwinrt -input Microsoft.Web.WebView2.Core.winmd sdk
    WORKING_DIRECTORY ${webview_SOURCE_DIR}/lib
)
file(COPY ${webview_SOURCE_DIR}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_library(sqlite ${sqlite_SOURCE_DIR}/sqlite3.c ${sqlite_SOURCE_DIR}/sqlite3.h)
set_target_properties(sqlite PROPERTIES LINKER_LANGUAGE C)

file(GLOB SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cxx)

add_executable(
    ${APP_NAME}
    WIN32
    ${SOURCES}
    ${RC_FILE}
    ${MANIFEST_FILE}
)

target_compile_definitions(
    ${APP_NAME}
    PUBLIC
    NLOHMANN_JSON_NAMESPACE_NO_VERSION=1
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(
        ${APP_NAME}
        PUBLIC
        /await:strict
        /bigobj
        /diagnostics:caret
        /EHsc
        /permissive-
        /utf-8
        /W4
        # /WX
        /Zc:__cplusplus
    )

    target_link_options(
        ${APP_NAME}
        PUBLIC
        # /WX
        /INCREMENTAL:NO
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(
        ${APP_NAME}
        PUBLIC
        -fcoroutines
    )
endif()

target_precompile_headers(
    ${APP_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/src/PCH.hxx
)

target_include_directories(
    ${APP_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}/config
    ${sqlite_SOURCE_DIR}
    ${wil_SOURCE_DIR}/lib
    ${webview_SOURCE_DIR}/lib
)

target_link_directories(
    ${APP_NAME}
    PUBLIC
    ${webview_SOURCE_DIR}/build/native/x64
)

target_link_libraries(
    ${APP_NAME}
    PUBLIC
    dwmapi.lib
    gdiplus.lib
    shcore.lib
    Comctl32.lib
    WebView2LoaderStatic.lib
    nlohmann_json::nlohmann_json
    sqlite
)

execute_process(
    COMMAND pwsh -nol -nop -noni -c pnpm build
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/gui
)

file(COPY ${CMAKE_SOURCE_DIR}/gui/dist DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(RENAME ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dist ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gui)
