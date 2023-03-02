cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(WEBVIEW_VER "1.0.1587.40")
set(CPPWINRT_VER "2.0.230225.1")
set(WIL_VER "1.0.230202.1")
set(DLL ${CMAKE_CURRENT_SOURCE_DIR}/build/libs/Microsoft.Web.WebView2.Core.dll)

project(Gooey VERSION 0.0.1 LANGUAGES CXX)

math(EXPR BITNESS "${CMAKE_SIZEOF_VOID_P} * 8" OUTPUT_FORMAT DECIMAL)
message(STATUS "CMake ${CMAKE_VERSION}, ${CMAKE_GENERATOR}, ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}, C++${CMAKE_CXX_STANDARD}")
message(STATUS "${PROJECT_NAME} (${BITNESS}-bit) v${PROJECT_VERSION}")
message(STATUS "${CMAKE_BUILD_TYPE} build")

file(COPY ${DLL} DESTINATION ${CMAKE_BINARY_DIR})

# add_executable(${PROJECT_NAME} WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/gooey.cpp ${CMAKE_CURRENT_SOURCE_DIR}/gooey.manifest ${CMAKE_CURRENT_SOURCE_DIR}/gooey.rc)
add_executable(${PROJECT_NAME} WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/gooey.cpp ${CMAKE_CURRENT_SOURCE_DIR}/gooey.rc)
target_precompile_headers(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/packages/Microsoft.Windows.ImplementationLibrary.${WIL_VER}/include)
# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/packages/Microsoft.Web.WebView2.${WEBVIEW_VER}/build/native/include)
# target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/packages/Microsoft.Web.WebView2.1.0.1587.40/build/native/x64/WebView2LoaderStatic.lib)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/libs/)
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/build/libs/WebView2LoaderStatic.lib)
