cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(Gooey VERSION 0.0.1 LANGUAGES CXX)

set(LIBS ${CMAKE_SOURCE_DIR}/external)
set(WEBVIEW "Microsoft.Web.WebView2.1.0.1661.34")
set(WIL "Microsoft.Windows.ImplementationLibrary.1.0.230202.1")

execute_process(COMMAND nuget install Microsoft.Web.WebView2 -OutputDirectory ${CMAKE_SOURCE_DIR}/external)
execute_process(COMMAND nuget install Microsoft.Windows.ImplementationLibrary -OutputDirectory ${CMAKE_SOURCE_DIR}/external)

math(EXPR BITNESS "${CMAKE_SIZEOF_VOID_P} * 8" OUTPUT_FORMAT DECIMAL)
message(STATUS "${PROJECT_NAME} (${BITNESS}-bit) [${CMAKE_BUILD_TYPE}] v${PROJECT_VERSION}, C++${CMAKE_CXX_STANDARD}, CMake ${CMAKE_VERSION}, ${CMAKE_GENERATOR}, ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
file(WRITE ${CMAKE_BINARY_DIR}/version.txt "v${PROJECT_VERSION}")

add_executable(${PROJECT_NAME} WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/gooey.cpp ${CMAKE_CURRENT_SOURCE_DIR}/gooey$<$<CONFIG:Debug>:_debug>.rc)

target_precompile_headers(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBS}/${WEBVIEW}/build/native/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBS}/${WIL}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS}/${WEBVIEW}/build/native/x64/WebView2LoaderStatic.lib)
target_link_libraries(${PROJECT_NAME} PUBLIC dwmapi.lib)
