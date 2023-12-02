cmake_minimum_required(VERSION 3.26)

set(APP_NAME
    "Airglow"
)
set(APP_VERSION
    "1.4.2"
)

project(
    ${APP_NAME}
    VERSION ${APP_VERSION}
    LANGUAGES C
              CXX
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/libs
)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/libs
)
set(CMAKE_PDB_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/pdb
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/${APP_NAME}
)

file(
    WRITE
    ${PROJECT_BINARY_DIR}/version.txt
    v${PROJECT_VERSION}
)
execute_process(
    COMMAND
        git --no-pager log -5 --oneline --no-decorate
    OUTPUT_FILE ${PROJECT_BINARY_DIR}/release_notes.txt
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

configure_file(
    ${PROJECT_SOURCE_DIR}/config/AirglowConfig.h.in
    ${PROJECT_BINARY_DIR}/AirglowConfig.h
)
# file(TO_CMAKE_PATH
# ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.rc RC_FILE)
file(
    TO_CMAKE_PATH
    ${PROJECT_SOURCE_DIR}/data/app.manifest
    MANIFEST_FILE
)

add_subdirectory(libs/Glow)

file(
    GLOB
    AIRGLOW_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cxx
)

file(
    GENERATE
    OUTPUT ${PROJECT_BINARY_DIR}/res.rc
    CONTENT "1 ICON \"$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico\""
)
file(
    TO_CMAKE_PATH
    ${PROJECT_BINARY_DIR}/res.rc
    RC_FILE
)

set(SOURCE_FILE
    ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico
)
set(OUTPUT_FILE
    ${PROJECT_BINARY_DIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico
)

add_custom_command(
    OUTPUT ${OUTPUT_FILE}
    COMMAND
        ${CMAKE_COMMAND} -E copy ${SOURCE_FILE} ${PROJECT_BINARY_DIR}
    COMMENT "Copying icon..."
)

add_custom_target(
    MakeIcon
    ALL
    DEPENDS ${OUTPUT_FILE}
)

add_executable(
    ${APP_NAME}
    WIN32
    ${AIRGLOW_SOURCES}
    ${MANIFEST_FILE}
    ${RC_FILE}
)

target_precompile_headers(
    ${APP_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/PCH.hxx
)

target_include_directories(
    ${APP_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
            ${PROJECT_BINARY_DIR}
)

target_link_libraries(
    ${APP_NAME}
    PRIVATE shcore.lib
            glow::glow
)

set(NUGET_DIRECTORY
    ${CMAKE_BINARY_DIR}/_deps/Nuget
)
set(WEBVIEW
    "Microsoft.Web.WebView2"
)
set(WEBVIEW_VER
    "1.0.2151.40"
)

add_custom_command(
    TARGET ${APP_NAME}
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${NUGET_DIRECTORY}/${WEBVIEW}.${WEBVIEW_VER}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Copying Microsoft.Web.WebView2.Core.dll..."
)

add_custom_target(
    BuildGui
    ALL
    COMMAND
        pnpm build
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/gui
)

add_custom_target(
    CopyGui
    ALL
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory dist ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gui
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/gui
)

add_dependencies(
    CopyGui
    BuildGui
)
add_dependencies(
    ${APP_NAME}
    MakeIcon
)
