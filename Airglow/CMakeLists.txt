add_executable(
    ${PROJECT_NAME}
    WIN32
)

file(
    TO_CMAKE_PATH
    ${PROJECT_BINARY_DIR}/data/Airglow.rc
    AIRGLOW_DATA_RC
)

file(
    GENERATE
    OUTPUT ${AIRGLOW_DATA_RC}
    CONTENT "1 ICON \"Airglow.ico\""
)

set(AIRGLOW_ICON_SOURCE
    ${PROJECT_SOURCE_DIR}/Data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico
)

set(AIRGLOW_ICON
    ${PROJECT_BINARY_DIR}/Data/Airglow.ico
)

add_custom_command(
    OUTPUT ${AIRGLOW_ICON}
    COMMAND ${CMAKE_COMMAND} -E copy ${AIRGLOW_ICON_SOURCE} ${AIRGLOW_ICON}
    COMMENT "Copying icon..."
)

add_custom_target(
    CopyIcon
    ALL
    DEPENDS ${AIRGLOW_ICON}
)

add_dependencies(
    ${PROJECT_NAME}
    CopyIcon
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE Main.cxx
            PCH.cxx
            App.cxx
            Storage.cxx
            Utility.cxx
            WebView.cxx
            ${AIRGLOW_DATA_RC}
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE FILE_SET
            HEADERS
            FILES
            App.hxx
            Definitions.hxx
            Storage.hxx
            Templates.hxx
            Utility.hxx
            WebView.hxx
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${PROJECT_BINARY_DIR}/Config
)

target_precompile_headers(
    ${PROJECT_NAME}
    PRIVATE
    PCH.hxx
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE shcore.lib
            glow::glow
            glow::flags
)

add_custom_target(
    BuildGui
    ALL
    COMMAND pnpm build
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/gui
)

add_custom_target(
    CopyGui
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory dist ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gui
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/gui
)

add_dependencies(
    CopyGui
    BuildGui
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/_deps/Nuget/${WEBVIEW}.${WEBVIEW_VER}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Copying Microsoft.Web.WebView2.Core.dll..."
)
