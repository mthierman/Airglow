cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
include(FetchContent)

option(USER_LIBS OFF)
option(USER_INSTALL OFF)
option(SYSTEM_INSTALL OFF)

project(
    Airglow
    VERSION 1.1.3
    LANGUAGES C CXX ASM
)

math(EXPR BITNESS "${CMAKE_SIZEOF_VOID_P} * 8" OUTPUT_FORMAT DECIMAL)
message(STATUS "${PROJECT_NAME} v${PROJECT_VERSION} (${BITNESS}-bit) [${CMAKE_BUILD_TYPE}]")
file(WRITE ${CMAKE_BINARY_DIR}/version.txt "${PROJECT_VERSION}")

set(WEBVIEW "Microsoft.Web.WebView2")
set(WEBVIEW_VER "1.0.1823.32")
set(WIL "Microsoft.Windows.ImplementationLibrary")
set(WIL_VER "1.0.230411.1")

if(USER_LIBS)
    file(TO_CMAKE_PATH $ENV{LIB_WEBVIEW} WEBVIEW_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_WIL} WIL_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_CHOC} CHOC_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_JSON} JSON_PKG)
    file(TO_CMAKE_PATH $ENV{LIB_SQLITE} SQLITE_PKG)
else()
    execute_process(
        COMMAND
        nuget install ${WEBVIEW} -Version ${WEBVIEW_VER} -OutputDirectory ${CMAKE_BINARY_DIR}/libs
    )
    file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/libs/${WEBVIEW}.${WEBVIEW_VER} WEBVIEW_PKG)

    execute_process(
        COMMAND
        nuget install ${WIL} -Version ${WIL_VER} -OutputDirectory ${CMAKE_BINARY_DIR}/libs
    )
    file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/libs/${WIL}.${WIL_VER} WIL_PKG)

    FetchContent_Declare(
        choc
        URL https://github.com/Tracktion/choc/archive/refs/heads/main.zip
    )
    FetchContent_MakeAvailable(choc)
    file(TO_CMAKE_PATH ${choc_SOURCE_DIR} CHOC_PKG)

    FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/include.zip
    )
    FetchContent_MakeAvailable(json)
    file(TO_CMAKE_PATH ${json_SOURCE_DIR}/single_include JSON_PKG)

    FetchContent_Declare(
        sqlite3
        URL https://www.sqlite.org/2023/sqlite-amalgamation-3420000.zip
    )
    FetchContent_MakeAvailable(sqlite3)
    file(TO_CMAKE_PATH ${sqlite3_SOURCE_DIR} SQLITE_PKG)
endif()

message(STATUS "WebView2: ${WEBVIEW_PKG}")
message(STATUS "Windows Implementation Library: ${WIL_PKG}")
message(STATUS "choc: ${CHOC_PKG}")
message(STATUS "nlohmann/json: ${JSON_PKG}")
message(STATUS "SQLite3: ${SQLITE_PKG}")

add_library(sqlite ${SQLITE_PKG}/sqlite3.c ${SQLITE_PKG}/sqlite3.h)
set_target_properties(sqlite PROPERTIES LINKER_LANGUAGE C)

add_executable(
    ${PROJECT_NAME}
    WIN32
    src/Airglow.cxx
    src/MainWindow.cxx
    src/Config.cxx
    src/Utility.cxx
    src/WebView.cxx
    src/Database.cxx
    res$<$<CONFIG:Debug>:_debug>$<$<CONFIG:Release>:_release>.rc
    app.manifest
)

target_precompile_headers(
    ${PROJECT_NAME}
    PRIVATE
    src/PCH.hxx
)

target_include_directories(
    ${PROJECT_NAME}
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
)

target_link_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${WEBVIEW_PKG}/build/native/x64
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    WebView2LoaderStatic.lib
    dwmapi.lib
    gdiplus.lib
    Uxtheme.lib
    sqlite
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    UNICODE
)

if(USER_INSTALL)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.exe $ENV{LocalAppData}/Airglow/${PROJECT_NAME}.exe
    )
endif()

if(SYSTEM_INSTALL)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.exe $ENV{ProgramFiles}/Airglow/${PROJECT_NAME}.exe
    )
endif()
