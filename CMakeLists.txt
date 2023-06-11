cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 23)

project(
    Gooey
    VERSION 0.0.1
    LANGUAGES CXX
)

set(WEBVIEW2 "Microsoft.Web.WebView2")
set(WEBVIEW2_VER "1.0.1823.32")

set(WIL "Microsoft.Windows.ImplementationLibrary")
set(WIL_VER "1.0.230411.1")

if(DEFINED ENV{WEBVIEW2})
    message(STATUS "WEBVIEW2 FOUND")
    set(WEBVIEW2_PKG $ENV{WEBVIEW2}/${WEBVIEW2_VER})
    message(STATUS ${WEBVIEW2_PKG})
else()
    execute_process(
        COMMAND
        nuget install ${WEBVIEW2} -Version ${WEBVIEW2_VER} -OutputDirectory ${CMAKE_SOURCE_DIR}/external
    )
    set(WEBVIEW_PKG external/${WEBVIEW2}.${WEBVIEW2_VER})
endif()

if(DEFINED ENV{WIL})
    message(STATUS "WIL FOUND")
    set(WIL_PKG $ENV{WIL}/${WIL_VER})
    message(STATUS ${WIL_PKG})
else()
    execute_process(
        COMMAND
        nuget install ${WIL} -Version ${WIL_VER} -OutputDirectory ${CMAKE_SOURCE_DIR}/external
    )
    set(WIL_PKG external/${WIL}.${WIL_VER})
endif()

set(WEBVIEW_VER "1.0.1823.32")

set(WIL_VER "1.0.230411.1")

math(EXPR BITNESS "${CMAKE_SIZEOF_VOID_P} * 8" OUTPUT_FORMAT DECIMAL)
message(STATUS "${PROJECT_NAME} v${PROJECT_VERSION} (${BITNESS}-bit) [${CMAKE_BUILD_TYPE}]")
file(WRITE ${CMAKE_BINARY_DIR}/version.txt "v${PROJECT_VERSION}")

add_executable(
    ${PROJECT_NAME}
    WIN32
    src/Gooey.cpp
    src/Gooey$<$<CONFIG:Debug>:_debug>$<$<CONFIG:Release>:_release>.rc
)

target_precompile_headers(
    ${PROJECT_NAME}
    PRIVATE
    src/pch.hpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${WEBVIEW2_PKG}/build/native/include
    ${WIL_PKG}/include
)

target_link_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${WEBVIEW2_PKG}/build/native/x64
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    WebView2LoaderStatic.lib
    dwmapi.lib
    Uxtheme.lib
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    UNICODE
)
