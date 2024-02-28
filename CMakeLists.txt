cmake_minimum_required(VERSION 3.27)

project(
    Airglow
    VERSION 1.7.1
    LANGUAGES C
              CXX
)

enable_testing()

include(FetchContent)

add_subdirectory(libs/Glow)

include(libs/Glow/cmake/ReleaseInfo.cmake)

file(
    DOWNLOAD
    "https://aka.ms/vs/17/release/vc_redist.x64.exe"
    "${PROJECT_BINARY_DIR}/vc_redist.x64.exe"
)

file(
    COPY ${PROJECT_SOURCE_DIR}/data/airglow.rc
    DESTINATION ${PROJECT_BINARY_DIR}/data
)

add_custom_command(
    OUTPUT ${PROJECT_BINARY_DIR}/data/icon.ico
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico
        ${PROJECT_BINARY_DIR}/data/icon.ico
    COMMENT "Copying icon..."
)

add_custom_target(
    Icon
    ALL
    DEPENDS ${PROJECT_BINARY_DIR}/data/icon.ico
)

add_custom_target(
    GUI
    ALL
    COMMAND pnpm build
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(
    Airglow
    WIN32
)

configure_file(
    ${PROJECT_SOURCE_DIR}/src/config.hxx
    ${PROJECT_BINARY_DIR}/include/airglow/config.hxx
)

target_sources(
    Airglow
    PRIVATE ${PROJECT_SOURCE_DIR}/src/app.cxx
            ${PROJECT_SOURCE_DIR}/src/browser.cxx
            ${PROJECT_SOURCE_DIR}/src/global.cxx
            ${PROJECT_SOURCE_DIR}/src/main.cxx
            ${PROJECT_SOURCE_DIR}/src/settings.cxx
            ${PROJECT_SOURCE_DIR}/src/window.cxx
            ${PROJECT_SOURCE_DIR}/data/airglow.manifest
            ${PROJECT_BINARY_DIR}/data/airglow.rc
    PRIVATE FILE_SET
            HEADERS
            BASE_DIRS
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_BINARY_DIR}/include
            FILES
            ${PROJECT_SOURCE_DIR}/src/app.hxx
            ${PROJECT_SOURCE_DIR}/src/browser.hxx
            ${PROJECT_SOURCE_DIR}/src/global.hxx
            ${PROJECT_SOURCE_DIR}/src/settings.hxx
            ${PROJECT_SOURCE_DIR}/src/state.hxx
            ${PROJECT_SOURCE_DIR}/src/window.hxx
            ${PROJECT_BINARY_DIR}/include/airglow/config.hxx
)

target_link_libraries(
    Airglow
    PRIVATE glow::glow
            glow::flags
)

add_dependencies(
    Airglow
    Icon
)
