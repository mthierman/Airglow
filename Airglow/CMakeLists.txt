include(${PROJECT_SOURCE_DIR}/CMake/Resources.cmake)

configure_file(
    ${PROJECT_SOURCE_DIR}/Config/airglow.hxx.in
    ${CMAKE_CURRENT_BINARY_DIR}/Include/config/airglow.hxx
)

add_executable(
    Airglow
    WIN32
)

target_sources(
    Airglow
    PRIVATE src/Main.cxx
            src/AddressBar.cxx
            src/App.cxx
            src/Browser.cxx
            src/Frame.cxx
            src/Settings.cxx
            src/SettingsWindow.cxx
            ${AIRGLOW_RC}
            ${PROJECT_SOURCE_DIR}/Data/airglow.manifest
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${CMAKE_CURRENT_SOURCE_DIR}
           ${CMAKE_CURRENT_BINARY_DIR}/Include
           FILES
           ${CMAKE_CURRENT_BINARY_DIR}/Include/config/airglow.hxx
           src/AddressBar.hxx
           src/App.hxx
           src/Browser.hxx
           src/Enums.hxx
           src/Frame.hxx
           src/Settings.hxx
           src/SettingsWindow.hxx
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

target_link_options(
    Airglow
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
    /entry:mainCRTStartup
    >
)
