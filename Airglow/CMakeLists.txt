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
            src/App.cxx
            src/Browser.cxx
            src/Settings.cxx
            ${AIRGLOW_RC}
            ${PROJECT_SOURCE_DIR}/Data/airglow.manifest
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${CMAKE_CURRENT_SOURCE_DIR}
           ${CMAKE_CURRENT_BINARY_DIR}/Include
           FILES
           ${CMAKE_CURRENT_BINARY_DIR}/Include/config/airglow.hxx
           src/App.hxx
           src/Browser.hxx
           src/Settings.hxx
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
