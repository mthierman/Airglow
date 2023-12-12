include(cmake/icon.cmake)

add_executable(
    ${PROJECT_NAME}
    WIN32
)

configure_file(
    config/airglow.hxx.in
    ${PROJECT_BINARY_DIR}/include/config/airglow.hxx
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE src/airglow.cxx
            src/app.cxx
            src/settings.cxx
            ${AIRGLOW_RC}
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${PROJECT_BINARY_DIR}/include
           ${CMAKE_CURRENT_SOURCE_DIR}
           FILES
           ${PROJECT_BINARY_DIR}/include/config/airglow.hxx
           src/app.hxx
           src/settings.hxx
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE glow::glow
            glow::flags
            # nlohmann_json::nlohmann_json tomlplusplus::tomlplusplus sqlite::sqlite dwmapi.lib
            # Comctl32.lib gdiplus.lib WebView2LoaderStatic.lib
)

add_dependencies(
    ${PROJECT_NAME}
    CopyIcon
)
