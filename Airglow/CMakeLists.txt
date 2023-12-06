add_executable(
    ${PROJECT_NAME}
    WIN32
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE Main.cxx
            PCH.cxx
            App.cxx
            Storage.cxx
            Utility.cxx
            WebView.cxx
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

# add_custom_command( TARGET ${APP_NAME} COMMAND ${CMAKE_COMMAND} -E copy
# ${NUGET_DIRECTORY}/${WEBVIEW}.${WEBVIEW_VER}/runtimes/win-x64/native_uap/Microsoft.Web.WebView2.Core.dll
# ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COMMENT "Copying Microsoft.Web.WebView2.Core.dll..." )

add_custom_target(
    BuildGui
    ALL
    COMMAND pnpm build
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/gui
)

add_custom_target(
    CopyGui
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory dist ${PROJECT_BINARY_DIR}/gui
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/gui
)

add_dependencies(
    CopyGui
    BuildGui
)
