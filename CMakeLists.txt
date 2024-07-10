cmake_minimum_required(VERSION 3.29)

project(Airglow VERSION 1.7.4)

add_subdirectory(libs/Glow)

list(
    APPEND
    CMAKE_MODULE_PATH
    "${PROJECT_SOURCE_DIR}/libs/Glow/libs/cmake-modules/"
    )

include(cmake-modules)

release_info()
download_vcredist()

file(COPY ${PROJECT_SOURCE_DIR}/data/airglow.rc DESTINATION ${PROJECT_BINARY_DIR}/data)

add_custom_command(
    OUTPUT ${PROJECT_BINARY_DIR}/data/icon.ico
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico
        ${PROJECT_BINARY_DIR}/data/icon.ico
    COMMENT "Copying icon..."
    )

add_custom_target(
    AirglowIcon
    ALL
    DEPENDS ${PROJECT_BINARY_DIR}/data/icon.ico
    )

add_custom_target(
    AirglowGui
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
            common::flags
    )

add_dependencies(
    Airglow
    AirglowIcon
    AirglowGui
    )
