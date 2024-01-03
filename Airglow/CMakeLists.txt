include(${PROJECT_SOURCE_DIR}/CMake/Resources.cmake)

configure_file(
    ${PROJECT_SOURCE_DIR}/config.hxx.in
    ${CMAKE_CURRENT_BINARY_DIR}/config/airglow/config.hxx
)

add_executable(
    Airglow
    WIN32
)

target_sources(
    Airglow
    PRIVATE airglow/main.cxx
            airglow/app.cxx
            airglow/window.cxx
            ${AIRGLOW_RC}
            ${PROJECT_SOURCE_DIR}/Data/airglow.manifest
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${PROJECT_SOURCE_DIR}/Airglow
           ${CMAKE_CURRENT_BINARY_DIR}/config
           FILES
           ${CMAKE_CURRENT_BINARY_DIR}/config/airglow/config.hxx
           airglow/definitions.hxx
           airglow/server.hxx
           airglow/app.hxx
           airglow/window.hxx
)

target_include_directories(
    Glow
    PUBLIC ${PROJECT_SOURCE_DIR}/Airglow
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
