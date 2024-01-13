cmake_minimum_required(VERSION 3.27)

project(
    Airglow
    VERSION 1.6.0
    LANGUAGES C
              CXX
)

add_subdirectory(libs/Glow)

include(${PROJECT_SOURCE_DIR}/CMake/Resources.cmake)

add_executable(
    Airglow
    WIN32
)

configure_file(
    ${PROJECT_SOURCE_DIR}/src/config.hxx
    ${CMAKE_CURRENT_BINARY_DIR}/include/airglow/config.hxx
)

target_sources(
    Airglow
    PRIVATE ${PROJECT_SOURCE_DIR}/src/app.cxx
            ${PROJECT_SOURCE_DIR}/src/browser.cxx
            ${PROJECT_SOURCE_DIR}/src/main.cxx
            ${PROJECT_SOURCE_DIR}/src/window.cxx
            ${AIRGLOW_RC}
            ${PROJECT_SOURCE_DIR}/Data/airglow.manifest
    PRIVATE FILE_SET
            HEADERS
            BASE_DIRS
            ${PROJECT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_BINARY_DIR}/include
            FILES
            ${PROJECT_SOURCE_DIR}/src/app.hxx
            ${PROJECT_SOURCE_DIR}/src/browser.hxx
            ${PROJECT_SOURCE_DIR}/src/global.hxx
            ${PROJECT_SOURCE_DIR}/src/window.hxx
            ${CMAKE_CURRENT_BINARY_DIR}/include/airglow/config.hxx
)

target_link_libraries(
    Airglow
    PRIVATE glow::glow
            glow::flags
)

add_dependencies(
    Airglow
    CopyIcon
)
