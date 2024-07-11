cmake_minimum_required(VERSION 3.29)

project(Airglow VERSION 1.7.5)

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

target_sources(
    Airglow
    PRIVATE "src/app.cxx"
            "src/browser.cxx"
            "src/global.cxx"
            "src/main.cxx"
            "src/settings.cxx"
            # "src/webview.cxx"
            "data/airglow.manifest"
            "${PROJECT_BINARY_DIR}/data/airglow.rc"
    )

target_link_libraries(
    Airglow
    PRIVATE glow::glow
            common::flags
            ada-url::ada
            nlohmann::json
    )

add_dependencies(
    Airglow
    AirglowIcon
    AirglowGui
    )
