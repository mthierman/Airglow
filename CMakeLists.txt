cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
include(FetchContent)

option(ENV_LIBS OFF)
option(INSTALL OFF)

set(APP_NAME "Airglow")
set(APP_VERSION "1.1.9")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${APP_NAME})

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

set(WEBVIEW "Microsoft.Web.WebView2")
set(WEBVIEW_VERSION "1.0.1823.32")
set(WIL "Microsoft.Windows.ImplementationLibrary")
set(WIL_VERSION "1.0.230629.1")

if(ENV_LIBS)
    file(TO_CMAKE_PATH $ENV{LIB_WEBVIEW} WEBVIEW_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_WIL} WIL_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_CHOC} CHOC_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_JSON} JSON_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_SQLITE} SQLITE_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_CURL} CURL_PKG)
else()
    execute_process(
        COMMAND
        nuget install ${WEBVIEW} -Version ${WEBVIEW_VERSION} -OutputDirectory ${CMAKE_BINARY_DIR}/_deps
    )
    file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/_deps/${WEBVIEW}.${WEBVIEW_VERSION} WEBVIEW_PKG)

    execute_process(
        COMMAND
        nuget install ${WIL} -Version ${WIL_VERSION} -OutputDirectory ${CMAKE_BINARY_DIR}/_deps
    )
    file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/_deps/${WIL}.${WIL_VERSION} WIL_PKG)

    FetchContent_Declare(
        choc
        URL https://github.com/Tracktion/choc/archive/refs/heads/main.zip
    )
    FetchContent_Populate(choc)
    file(TO_CMAKE_PATH ${choc_SOURCE_DIR} CHOC_PKG)

    FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/include.zip
    )
    FetchContent_Populate(json)
    file(TO_CMAKE_PATH ${json_SOURCE_DIR}/single_include JSON_PKG)

    FetchContent_Declare(
        sqlite3
        URL https://www.sqlite.org/2023/sqlite-amalgamation-3420000.zip
    )
    FetchContent_Populate(sqlite3)
    file(TO_CMAKE_PATH ${sqlite3_SOURCE_DIR} SQLITE_PKG)

    FetchContent_Declare(
        curl
        URL https://github.com/curl/curl/releases/download/curl-8_1_2/curl-8.1.2.zip
    )
    FetchContent_Populate(curl)
    file(TO_CMAKE_PATH ${curl_SOURCE_DIR} CURL_PKG)
endif()

message(STATUS "Libraries found:\n")
message(STATUS "${WEBVIEW}.${WEBVIEW_VERSION}: ${WEBVIEW_PKG}\n")
message(STATUS "${WIL}.${WIL_VERSION}: ${WIL_PKG}\n")
message(STATUS "choc: ${CHOC_PKG}\n")
message(STATUS "nlohmann/json: ${JSON_PKG}\n")
message(STATUS "SQLite3: ${SQLITE_PKG}\n")
message(STATUS "curl: ${CURL_PKG}\n")

add_library(sqlite ${SQLITE_PKG}/sqlite3.c ${SQLITE_PKG}/sqlite3.h)
set_target_properties(sqlite PROPERTIES LINKER_LANGUAGE C)

file(GLOB SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cxx)
file(TO_CMAKE_PATH ${CMAKE_SOURCE_DIR}/res$<$<CONFIG:Debug>:_debug>$<$<CONFIG:Release>:_release>.rc RC_FILE)
file(TO_CMAKE_PATH ${CMAKE_SOURCE_DIR}/app.manifest MANIFEST_FILE)
file(TO_CMAKE_PATH ${CMAKE_SOURCE_DIR}/src/PCH.hxx PCH_FILE)

add_executable(
    ${APP_NAME}
    WIN32
    ${SOURCES}
    ${RC_FILE}
    ${MANIFEST_FILE}
)

target_compile_definitions(
    ${APP_NAME}
    PRIVATE
    UNICODE
)

target_precompile_headers(
    ${APP_NAME}
    PRIVATE
    ${PCH_FILE}
)

target_include_directories(
    ${APP_NAME}
    PRIVATE
    ${WEBVIEW_PKG}/build/native/include
    ${WIL_PKG}/include
    ${CHOC_PKG}/audio
    ${CHOC_PKG}/containers
    ${CHOC_PKG}/gui
    ${CHOC_PKG}/javascript
    ${CHOC_PKG}/math
    ${CHOC_PKG}/memory
    ${CHOC_PKG}/platform
    ${CHOC_PKG}/text
    ${CHOC_PKG}/threading
    ${JSON_PKG}
    ${SQLITE_PKG}
    ${CURL_PKG}/include/curl
)

target_link_directories(
    ${APP_NAME}
    PRIVATE
    ${WEBVIEW_PKG}/build/native/x64
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
    sqlite
)

add_custom_command(
    TARGET ${APP_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/tools/install.ps1" "${CMAKE_BINARY_DIR}/install.ps1"
    COMMAND cd "${CMAKE_SOURCE_DIR}/gui" && pnpm run build
    COMMAND 7z a "${CMAKE_BINARY_DIR}/Airglow.zip" "${CMAKE_BINARY_DIR}/${APP_NAME}/*"
)

add_custom_command(
    TARGET ${APP_NAME} POST_BUILD
    COMMAND 7z a "${CMAKE_BINARY_DIR}/Airglow.zip" "${CMAKE_BINARY_DIR}/${APP_NAME}/${APP_NAME}.exe" "${CMAKE_BINARY_DIR}/${APP_NAME}/settings"
)

if(INSTALL)
    add_custom_command(
        TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/${APP_NAME}" "$ENV{LocalAppData}/${APP_NAME}"
    )
endif()
