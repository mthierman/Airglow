cmake_minimum_required(VERSION 3.26)
set(APP_NAME "Airglow")
set(APP_NAME_CONSOLE "Isochasm")
set(APP_VERSION "1.3.0")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${APP_NAME}")

include(FetchContent)

option(ENV_LIBS OFF)
option(EXPERIMENTAL_LIBS OFF)
option(INSTALL OFF)

project(
    ${APP_NAME}
    VERSION ${APP_VERSION}
    LANGUAGES C CXX ASM
)

message(STATUS "")
math(EXPR BITNESS "${CMAKE_SIZEOF_VOID_P} * 8" OUTPUT_FORMAT DECIMAL)
message(STATUS "${APP_NAME} v${APP_VERSION} (${BITNESS}-bit) [${CMAKE_BUILD_TYPE}]")
message(STATUS "")
file(WRITE "${CMAKE_BINARY_DIR}/app_version.txt" "v${APP_VERSION}")
execute_process(
    COMMAND
    git --no-pager log -5 --oneline --no-decorate
    OUTPUT_FILE "${CMAKE_BINARY_DIR}/release_notes.txt"
)

if(EXPERIMENTAL_LIBS)
    set(WEBVIEW "Microsoft.Web.WebView2")
    set(WEBVIEW_VERSION "1.0.1988-prerelease")
else()
    set(WEBVIEW "Microsoft.Web.WebView2")
    set(WEBVIEW_VERSION "1.0.1901.177")
endif()

set(WIL "Microsoft.Windows.ImplementationLibrary")
set(WIL_VERSION "1.0.230629.1")

if(ENV_LIBS)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/${WEBVIEW}.${WEBVIEW_VERSION}" WEBVIEW_PKG)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/${WIL}.${WIL_VERSION}" WIL_PKG)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/choc" CHOC_PKG)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/nlohmann/json/single_include" JSON_PKG)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/sqlite-amalgamation-3420000" SQLITE_PKG)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/curl" CURL_PKG)
else()
    execute_process(
        COMMAND
        nuget install "${WEBVIEW}" -Version "${WEBVIEW_VERSION}" -OutputDirectory "${CMAKE_BINARY_DIR}/_deps"
    )
    file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/_deps/${WEBVIEW}.${WEBVIEW_VERSION}" WEBVIEW_PKG)

    execute_process(
        COMMAND
        nuget install "${WIL}" -Version "${WIL_VERSION}" -OutputDirectory "${CMAKE_BINARY_DIR}/_deps"
    )
    file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/_deps/${WIL}.${WIL_VERSION}" WIL_PKG)

    FetchContent_Declare(
        choc
        URL https://github.com/Tracktion/choc/archive/refs/heads/main.zip
    )
    FetchContent_Populate(choc)
    file(TO_CMAKE_PATH "${choc_SOURCE_DIR}" CHOC_PKG)

    FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/include.zip
    )
    FetchContent_Populate(json)
    file(TO_CMAKE_PATH "${json_SOURCE_DIR}/single_include" JSON_PKG)

    FetchContent_Declare(
        sqlite3
        URL https://www.sqlite.org/2023/sqlite-amalgamation-3420000.zip
    )
    FetchContent_Populate(sqlite3)
    file(TO_CMAKE_PATH "${sqlite3_SOURCE_DIR}" SQLITE_PKG)

    FetchContent_Declare(
        curl
        URL https://github.com/curl/curl/releases/download/curl-8_2_1/curl-8.2.1.zip
    )
    FetchContent_Populate(curl)
    file(TO_CMAKE_PATH "${curl_SOURCE_DIR}" CURL_PKG)
endif()

message(STATUS "Libraries found:\n")
message(STATUS "${WEBVIEW}.${WEBVIEW_VERSION}: ${WEBVIEW_PKG}\n")
message(STATUS "${WIL}.${WIL_VERSION}: ${WIL_PKG}\n")
message(STATUS "choc: ${CHOC_PKG}\n")
message(STATUS "nlohmann/json: ${JSON_PKG}\n")
message(STATUS "SQLite3: ${SQLITE_PKG}\n")
message(STATUS "curl: ${CURL_PKG}\n")

add_library(sqlite "${SQLITE_PKG}/sqlite3.c" "${SQLITE_PKG}/sqlite3.h")
set_target_properties(sqlite PROPERTIES LINKER_LANGUAGE C)

file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cxx")
file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/res$<$<CONFIG:Debug>:_debug>$<$<CONFIG:Release>:_release>.rc" RC_FILE)
file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/app.manifest" MANIFEST_FILE)
file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/src/PCH.hxx" PCH_FILE)

add_executable(
    ${APP_NAME}
    WIN32
    "${SOURCES}"
    "${RC_FILE}"
    "${MANIFEST_FILE}"
)

add_executable(
    ${APP_NAME_CONSOLE}
    "${CMAKE_SOURCE_DIR}/src/console/Main.cxx"
)

target_compile_definitions(
    ${APP_NAME}
    PRIVATE
    UNICODE
    APP_NAME="${APP_NAME}"
    APP_NAME_CONSOLE="${APP_NAME_CONSOLE}"
    APP_VERSION="${APP_VERSION}"
    NLOHMANN_JSON_NAMESPACE_NO_VERSION=1
)

target_compile_options(
    ${APP_NAME}
    PRIVATE
    /Zc:__cplusplus
)

target_precompile_headers(
    ${APP_NAME}
    PRIVATE
    "${PCH_FILE}"
)

target_include_directories(
    ${APP_NAME}
    PRIVATE
    "${WEBVIEW_PKG}/build/native/include"
    "${WIL_PKG}/include"
    "${CHOC_PKG}/audio"
    "${CHOC_PKG}/containers"
    "${CHOC_PKG}/gui"
    "${CHOC_PKG}/javascript"
    "${CHOC_PKG}/math"
    "${CHOC_PKG}/memory"
    "${CHOC_PKG}/platform"
    "${CHOC_PKG}/text"
    "${CHOC_PKG}/threading"
    "${JSON_PKG}"
    "${SQLITE_PKG}"
    "${CURL_PKG}/include/curl"
)

target_link_directories(
    ${APP_NAME}
    PRIVATE
    "${WEBVIEW_PKG}/build/native/x64"
)

target_link_libraries(
    ${APP_NAME}
    PRIVATE
    WebView2LoaderStatic.lib
    dwmapi.lib
    gdiplus.lib
    Uxtheme.lib
    Urlmon.lib
    Kernel32.lib
    User32.lib
    sqlite
)

configure_file("${CMAKE_SOURCE_DIR}/tools/install.ps1" "${CMAKE_BINARY_DIR}/install.ps1")

add_custom_command(
    TARGET ${APP_NAME} PRE_BUILD
    COMMAND cd "${CMAKE_SOURCE_DIR}/gui" && pnpm build
)

add_custom_command(
    TARGET ${APP_NAME} POST_BUILD
    COMMAND 7z a "${CMAKE_BINARY_DIR}/Airglow.zip" "${CMAKE_BINARY_DIR}/${APP_NAME}/${APP_NAME}.exe" "${CMAKE_BINARY_DIR}/${APP_NAME}/gui"
)

if(INSTALL)
    add_custom_command(
        TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/${APP_NAME}" "$ENV{LocalAppData}/${APP_NAME}"
    )
endif()
