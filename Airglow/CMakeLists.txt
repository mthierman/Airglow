add_executable(
    ${NAME}
    WIN32
)

target_sources(
    ${NAME}
    PRIVATE App.cxx
            Main.cxx
            PCH.cxx
            Storage.cxx
            Utility.cxx
            WebView.cxx
)

target_sources(
    Glow
    PUBLIC FILE_SET
           HEADERS
           FILES
           App.hxx
           Definitions.hxx
           Main.hxx
           Storage.hxx
           Templates.hxx
           Utility.hxx
           WebView.hxx
)

target_precompile_headers(
    ${NAME}
    PRIVATE
    PCH.hxx
)

target_link_libraries(
    ${NAME}
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
add_dependencies(
    ${APP_NAME}
    MakeIcon
)
