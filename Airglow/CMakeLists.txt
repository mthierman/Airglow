include(${PROJECT_SOURCE_DIR}/cmake/icon.cmake)

add_executable(
    ${PROJECT_NAME}
    WIN32
)

configure_file(
    ${PROJECT_SOURCE_DIR}/config/airglow.hxx.in
    ${PROJECT_BINARY_DIR}/include/config/airglow.hxx
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE src/airglow.cxx
            src/app.cxx
            src/webview.cxx
            ${AIRGLOW_RC}
            ${PROJECT_SOURCE_DIR}/data/airglow.manifest
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${CMAKE_CURRENT_SOURCE_DIR}
           ${PROJECT_BINARY_DIR}/include
           FILES
           ${PROJECT_BINARY_DIR}/include/config/airglow.hxx
           src/app.hxx
           src/webview.hxx
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
