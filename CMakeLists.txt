cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
include(FetchContent)

option(USER_LIBS OFF)
option(USER_INSTALL OFF)
option(SYSTEM_INSTALL OFF)

file(READ ${CMAKE_SOURCE_DIR}/Airglow.json PROJECT_JSON)
string(JSON APP_NAME GET ${PROJECT_JSON} "name")
string(JSON APP_VERSION GET ${PROJECT_JSON} "version")
string(JSON WEBVIEW GET ${PROJECT_JSON} "webview")
string(JSON WEBVIEW_VER GET ${PROJECT_JSON} "webview_ver")
string(JSON WIL GET ${PROJECT_JSON} "wil")
string(JSON WIL_VER GET ${PROJECT_JSON} "wil_ver")

project(
    ${APP_NAME}
    VERSION ${APP_VERSION}
    LANGUAGES C CXX ASM
)

math(EXPR BITNESS "${CMAKE_SIZEOF_VOID_P} * 8" OUTPUT_FORMAT DECIMAL)

message(STATUS "${APP_NAME} v${APP_VERSION} (${BITNESS}-bit) [${CMAKE_BUILD_TYPE}]")

if(USER_LIBS)
    file(TO_CMAKE_PATH $ENV{LIB_WEBVIEW} WEBVIEW_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_WIL} WIL_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_CHOC} CHOC_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_JSON} JSON_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_SQLITE} SQLITE_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_CURL} CURL_PKG)
else()
    execute_process(
        COMMAND
        nuget install ${WEBVIEW} -Version ${WEBVIEW_VER} -OutputDirectory ${CMAKE_BINARY_DIR}/_deps
    )
    file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/_deps/${WEBVIEW}.${WEBVIEW_VER} WEBVIEW_PKG)

    execute_process(
        COMMAND
        nuget install ${WIL} -Version ${WIL_VER} -OutputDirectory ${CMAKE_BINARY_DIR}/_deps
    )
    file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/_deps/${WIL}.${WIL_VER} WIL_PKG)

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

message(STATUS "${WEBVIEW}.${WEBVIEW_VER}: ${WEBVIEW_PKG}")
message(STATUS "${WIL}.${WIL_VER}: ${WIL_PKG}")
message(STATUS "choc: ${CHOC_PKG}")
message(STATUS "nlohmann/json: ${JSON_PKG}")
message(STATUS "SQLite3: ${SQLITE_PKG}")
message(STATUS "curl: ${CURL_PKG}")

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
    sqlite
)

add_custom_command(
    TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/tools/install.ps1 ${CMAKE_BINARY_DIR}/Install.ps1
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/Airglow.json ${CMAKE_BINARY_DIR}/Airglow.json
    COMMAND cd ${CMAKE_SOURCE_DIR}/gui && pnpm run build
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/gui/dist ${CMAKE_BINARY_DIR}/gui
)

if(USER_INSTALL)
    add_custom_command(
        TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${APP_NAME}.exe $ENV{LocalAppData}/Airglow/${APP_NAME}.exe
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/gui $ENV{LocalAppData}/Airglow/gui
    )
endif()

if(SYSTEM_INSTALL)
    add_custom_command(
        TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${APP_NAME}.exe $ENV{ProgramFiles}/Airglow/${APP_NAME}.exe
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/gui $ENV{LocalAppData}/Airglow/gui
    )
endif()
