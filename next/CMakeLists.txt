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
    PRIVATE src/Main.cxx
            ${PROJECT_SOURCE_DIR}/data/airglow.manifest
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${PROJECT_BINARY_DIR}/include
           FILES
           ${PROJECT_BINARY_DIR}/include/config/airglow.hxx
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE glow::glow
            glow::flags
)
