file(
    TO_CMAKE_PATH
    ${PROJECT_SOURCE_DIR}/data/Airglow.manifest
    AIRGLOW_DATA_MANIFEST
)

configure_file(
    ${PROJECT_SOURCE_DIR}/config/AirglowConfig.h.in
    ${PROJECT_BINARY_DIR}/config/AirglowConfig.h
)

file(
    TO_CMAKE_PATH
    ${PROJECT_BINARY_DIR}/config
    GLOW_CONFIG_PATH
)
