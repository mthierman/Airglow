include(${PROJECT_SOURCE_DIR}/CMake/Resources.cmake)

configure_file(
    ${PROJECT_SOURCE_DIR}/Config/airglow.hxx.in
    ${CMAKE_CURRENT_BINARY_DIR}/Include/config/airglow.hxx
)

add_executable(
    ${PROJECT_NAME}
    WIN32
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE Airglow.cxx
            App.cxx
            WebView.cxx
            ${AIRGLOW_RC}
            ${PROJECT_SOURCE_DIR}/Data/airglow.manifest
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${CMAKE_CURRENT_SOURCE_DIR}
           ${CMAKE_CURRENT_BINARY_DIR}/Include
           FILES
           ${CMAKE_CURRENT_BINARY_DIR}/Include/config/airglow.hxx
           App.hxx
           WebView.hxx
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE glow::glow
            glow::flags
)

add_dependencies(
    ${PROJECT_NAME}
    CopyIcon
)
