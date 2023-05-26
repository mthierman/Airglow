cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)
set(
    CMAKE_TRY_COMPILE_TARGET_TYPE
    STATIC_LIBRARY
)

project(
    Gooey
    VERSION 0.0.1
    LANGUAGES CXX
)

set(
    EXTERNAL
    ${CMAKE_SOURCE_DIR}/external
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
    nuget install ${WEBVIEW} -Version ${WEBVIEW_VER} -OutputDirectory ${EXTERNAL}
)

execute_process(
    COMMAND
    nuget install ${WIL} -Version ${WIL_VER} -OutputDirectory ${EXTERNAL}
)

add_executable(
    ${PROJECT_NAME}
    WIN32
    ${CMAKE_SOURCE_DIR}/src/gooey.cpp
    ${CMAKE_SOURCE_DIR}/gooey$<$<CONFIG:Debug>:_debug>$<$<CONFIG:Release>:_release>.rc

    # ${CMAKE_SOURCE_DIR}/gooey.rc
)

target_precompile_headers(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/src/pch.hpp
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${EXTERNAL}/${WEBVIEW_LIB}/build/native/include
    ${EXTERNAL}/${WIL_LIB}/include
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    ${EXTERNAL}/${WEBVIEW_LIB}/build/native/x64/WebView2LoaderStatic.lib
    dwmapi.lib
)
