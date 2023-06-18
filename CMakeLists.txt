cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(
    Airglow
    VERSION 1.0.0
    LANGUAGES CXX
)

math(EXPR BITNESS "${CMAKE_SIZEOF_VOID_P} * 8" OUTPUT_FORMAT DECIMAL)
message(STATUS "${PROJECT_NAME} v${PROJECT_VERSION} (${BITNESS}-bit) [${CMAKE_BUILD_TYPE}]")
file(WRITE ${CMAKE_BINARY_DIR}/version.txt "v${PROJECT_VERSION}")

include(FetchContent)

set(WEBVIEW "Microsoft.Web.WebView2")
set(WEBVIEW_VER "1.0.1823.32")

if(DEFINED ENV{LIB_WEBVIEW})
    set(WEBVIEW_PKG $ENV{LIB_WEBVIEW})
else()
    execute_process(
        COMMAND
        nuget install ${WEBVIEW} -Version ${WEBVIEW_VER} -OutputDirectory ${CMAKE_BINARY_DIR}/libs
    )
    set(WEBVIEW_PKG ${CMAKE_BINARY_DIR}/libs/${WEBVIEW}.${WEBVIEW_VER})
endif()

set(WIL "Microsoft.Windows.ImplementationLibrary")
set(WIL_VER "1.0.230411.1")

if(DEFINED ENV{LIB_WIL})
    set(WIL_PKG $ENV{LIB_WIL})
else()
    execute_process(
        COMMAND
        nuget install ${WIL} -Version ${WIL_VER} -OutputDirectory ${CMAKE_BINARY_DIR}/libs
    )
    set(WIL_PKG ${CMAKE_BINARY_DIR}/libs/${WIL}.${WIL_VER})
endif()

if(DEFINED ENV{LIB_CHOC})
    set(CHOC_PKG $ENV{LIB_CHOC})
else()
    FetchContent_Declare(
        choc
        URL https://github.com/Tracktion/choc/archive/refs/heads/main.zip
    )
    FetchContent_MakeAvailable(choc)
    set(CHOC_PKG ${choc_SOURCE_DIR})
endif()

if(DEFINED ENV{LIB_JSON})
    set(JSON_PKG $ENV{LIB_JSON})
else()
    FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/include.zip
    )
    FetchContent_MakeAvailable(json)
    set(JSON_PKG ${json_SOURCE_DIR}/single_include/nlohmann)
endif()

message(STATUS ${WEBVIEW_PKG})
message(STATUS ${WIL_PKG})
message(STATUS ${CHOC_PKG})
message(STATUS ${JSON_PKG})

add_executable(
    ${PROJECT_NAME}
    WIN32
    src/Main.cpp
    res$<$<CONFIG:Debug>:_debug>$<$<CONFIG:Release>:_release>.rc
)

target_precompile_headers(
    ${PROJECT_NAME}
    PRIVATE
    src/pch.hpp
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
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    UNICODE
)
