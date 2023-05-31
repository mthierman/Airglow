cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 23)

project(
    Gooey
    VERSION 0.0.1
    LANGUAGES CXX
)

set(WEBVIEW "Microsoft.Web.WebView2")
set(WEBVIEW_VER "1.0.1774.30")
set(WEBVIEW_LIB "${WEBVIEW}.${WEBVIEW_VER}")
set(WEBVIEW_DLL WebView2Loader.dll)

set(WIL "Microsoft.Windows.ImplementationLibrary")
set(WIL_VER "1.0.230411.1")
set(WIL_LIB "${WIL}.${WIL_VER}")

math(EXPR BITNESS "${CMAKE_SIZEOF_VOID_P} * 8" OUTPUT_FORMAT DECIMAL)
message(STATUS "${PROJECT_NAME} v${PROJECT_VERSION} (${BITNESS}-bit) [${CMAKE_BUILD_TYPE}]")
file(WRITE ${CMAKE_BINARY_DIR}/version.txt "v${PROJECT_VERSION}")

execute_process(
    COMMAND
    nuget install ${WEBVIEW} -Version ${WEBVIEW_VER} -OutputDirectory ${CMAKE_SOURCE_DIR}/external
)

execute_process(
    COMMAND
    nuget install ${WIL} -Version ${WIL_VER} -OutputDirectory ${CMAKE_SOURCE_DIR}/external
)

add_executable(
    ${PROJECT_NAME}
    WIN32
    src/gooey.cpp
    gooey$<$<CONFIG:Debug>:_debug>$<$<CONFIG:Release>:_release>.rc
)

target_precompile_headers(
    ${PROJECT_NAME}
    PRIVATE
    src/pch.hpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    external/${WEBVIEW_LIB}/build/native/include
    external/${WIL_LIB}/include
)

target_link_directories(
    ${PROJECT_NAME}
    PRIVATE
    external/${WEBVIEW_LIB}/build/native/x64
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    WebView2LoaderStatic.lib
    dwmapi.lib
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    UNICODE
)
