cmake_minimum_required(VERSION 3.26)
set(APP_NAME "Airglow")
set(APP_VERSION "1.4.2")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${APP_NAME}")

include(FetchContent)

option(ENV_LIBS OFF)
option(INSTALL OFF)
option(CREATE_SYMLINKS OFF)

project(
    ${APP_NAME}
    VERSION ${APP_VERSION}
    LANGUAGES C CXX ASM
)

math(EXPR BITNESS "${CMAKE_SIZEOF_VOID_P} * 8" OUTPUT_FORMAT DECIMAL)
message(STATUS "${APP_NAME} v${APP_VERSION} (${BITNESS}-bit) [${CMAKE_BUILD_TYPE}]")
file(WRITE "${CMAKE_BINARY_DIR}/app_version.txt" "v${APP_VERSION}")
execute_process(
    COMMAND
    git --no-pager log -5 --oneline --no-decorate
    OUTPUT_FILE "${CMAKE_BINARY_DIR}/release_notes.txt"
)

set(WEBVIEW "Microsoft.Web.WebView2")
set(WEBVIEW_VERSION "1.0.2045.28")

set(CPPWINRT "Microsoft.Windows.CppWinRT")
set(CPPWINRT_VER "2.0.230706.1")

set(WIL "Microsoft.Windows.ImplementationLibrary")
set(WIL_VERSION "1.0.230824.2")

if(ENV_LIBS)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/${WEBVIEW}.${WEBVIEW_VERSION}" WEBVIEW_PKG)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/${CPPWINRT}.${CPPWINRT_VER}" CPPWINRT_PKG)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/${WIL}.${WIL_VERSION}" WIL_PKG)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/nlohmann/json/single_include" JSON_PKG)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/sqlite-amalgamation-3430100" SQLITE_PKG)
    file(TO_CMAKE_PATH "$ENV{LIBRARIES}/curl" CURL_PKG)
else()
    execute_process(
        COMMAND
        nuget install "${WEBVIEW}" -Version "${WEBVIEW_VERSION}" -OutputDirectory "${CMAKE_BINARY_DIR}/_deps"
    )
    file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/_deps/${WEBVIEW}.${WEBVIEW_VERSION}" WEBVIEW_PKG)

    execute_process(
        COMMAND
        nuget install "${CPPWINRT}" -Version "${CPPWINRT_VER}" -OutputDirectory "${CMAKE_BINARY_DIR}/_deps"
    )
    file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/_deps/${CPPWINRT}.${CPPWINRT_VER}" CPPWINRT_PKG)

    execute_process(
        COMMAND
        nuget install "${WIL}" -Version "${WIL_VERSION}" -OutputDirectory "${CMAKE_BINARY_DIR}/_deps"
    )
    file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/_deps/${WIL}.${WIL_VERSION}" WIL_PKG)

    FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/include.zip
    )
    FetchContent_Populate(json)
    file(TO_CMAKE_PATH "${json_SOURCE_DIR}/single_include" JSON_PKG)

    FetchContent_Declare(
        sqlite3
        URL https://www.sqlite.org/2023/sqlite-amalgamation-3430100.zip
    )
    FetchContent_Populate(sqlite3)
    file(TO_CMAKE_PATH "${sqlite3_SOURCE_DIR}" SQLITE_PKG)

    FetchContent_Declare(
        curl
        URL https://github.com/curl/curl/releases/download/curl-8_3_0/curl-8.3.0.zip
    )
    FetchContent_Populate(curl)
    file(TO_CMAKE_PATH "${curl_SOURCE_DIR}" CURL_PKG)
endif()

message(STATUS "WebView2 ${WEBVIEW_PKG}")
message(STATUS "C++/WinRT ${CPPWINRT_PKG}")
message(STATUS "WIL ${WIL_PKG}")
message(STATUS "json ${JSON_PKG}")
message(STATUS "SQLite3 ${SQLITE_PKG}")
message(STATUS "curl ${CURL_PKG}")

add_library(sqlite "${SQLITE_PKG}/sqlite3.c" "${SQLITE_PKG}/sqlite3.h")
set_target_properties(sqlite PROPERTIES LINKER_LANGUAGE C)

execute_process(
    COMMAND cppwinrt -input Microsoft.Web.WebView2.Core.winmd sdk WORKING_DIRECTORY "${WEBVIEW_PKG}/lib"
)

file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cxx")
file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/res$<$<CONFIG:Debug>:_debug>$<$<CONFIG:Release>:_release>.rc" RC_FILE)
file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/app.manifest" MANIFEST_FILE)
file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/src/include/PCH.hxx" PCH_FILE)

add_executable(
    ${APP_NAME}
    WIN32
    "${SOURCES}"
    "${RC_FILE}"
    "${MANIFEST_FILE}"
)

add_compile_definitions(
    UNICODE
    APP_NAME="${APP_NAME}"
    APP_NAME_CONSOLE="${APP_NAME_CONSOLE}"
    APP_VERSION="${APP_VERSION}"
    NLOHMANN_JSON_NAMESPACE_NO_VERSION=1
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
        /EHsc
        /Zc:__cplusplus
        /await:strict
        /bigobj
        /permissive-
        /utf-8
        /W4
        /WX
    )

    add_link_options(
        /WX
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
        -fcoroutines
    )
endif()

target_precompile_headers(
    ${APP_NAME}
    PRIVATE
    "${PCH_FILE}"
)

target_include_directories(
    ${APP_NAME}
    PRIVATE
    "${WEBVIEW_PKG}/lib"
    "${WIL_PKG}/include"
    "${JSON_PKG}"
    "${SQLITE_PKG}"
    "${CURL_PKG}/include/curl"
)

target_link_libraries(
    ${APP_NAME}
    PRIVATE
    dwmapi.lib
    gdiplus.lib
    shcore.lib
    sqlite
)

configure_file("${CMAKE_SOURCE_DIR}/tools/install.ps1" "${CMAKE_BINARY_DIR}/install.ps1")

add_custom_command(
    TARGET ${APP_NAME} PRE_BUILD
    COMMAND cd "${CMAKE_SOURCE_DIR}/gui" && pnpm build
)

add_custom_command(
    TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${WEBVIEW_PKG}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMAND 7z a "${CMAKE_BINARY_DIR}/Airglow.zip" "${CMAKE_BINARY_DIR}/${APP_NAME}/${APP_NAME}.exe" "${CMAKE_BINARY_DIR}/${APP_NAME}/Microsoft.Web.WebView2.Core.dll" "${CMAKE_BINARY_DIR}/${APP_NAME}/gui"
)

if(INSTALL)
    add_custom_command(
        TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/${APP_NAME}" "$ENV{LocalAppData}/${APP_NAME}"
    )
endif()

if(CREATE_SYMLINKS)
    add_custom_command(
        TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_BINARY_DIR}/${APP_NAME}" "$ENV{LocalAppData}/${APP_NAME}"
    )
endif()
