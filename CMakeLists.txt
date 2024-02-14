cmake_minimum_required(VERSION 3.27)

project(
    Airglow
    VERSION 1.6.1
    LANGUAGES C
              CXX
)

option(
    BUILD_GUI
    ON
)

add_subdirectory(libs/Glow)

file(
    WRITE
    ${PROJECT_BINARY_DIR}/notes/version
    v${PROJECT_VERSION}
)

execute_process(
    COMMAND git rev-parse --short HEAD
    OUTPUT_FILE ${PROJECT_BINARY_DIR}/notes/short_hash
)

execute_process(
    COMMAND git --no-pager log -5 --oneline --no-decorate
    OUTPUT_FILE ${PROJECT_BINARY_DIR}/notes/release_notes
)

file(
    GENERATE
    OUTPUT ${PROJECT_BINARY_DIR}/data/airglow.rc
    CONTENT "1 ICON \"icon.ico\""
)

add_custom_command(
    OUTPUT ${PROJECT_BINARY_DIR}/data/icon.ico
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico
        ${PROJECT_BINARY_DIR}/data/icon.ico
    COMMENT "Copying icon..."
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

add_custom_target(
    CopyIcon
    ALL
    DEPENDS ${PROJECT_BINARY_DIR}/data/icon.ico
)

add_dependencies(
    Airglow
    CopyIcon
)

if(BUILD_GUI)
    add_custom_target(
        Vite
        COMMAND pnpm build
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

    add_dependencies(
        Airglow
        Vite
    )
endif()

if(GLOW_EXTRAS)
    enable_testing()
endif()
