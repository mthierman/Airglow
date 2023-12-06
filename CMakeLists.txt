cmake_minimum_required(VERSION 3.27)

project(
    Airglow
    VERSION 1.5.0
    LANGUAGES C
              CXX
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config/AirglowConfig.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config/AirglowConfig.h
)

file(
    TO_CMAKE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Airglow.manifest
    AIRGLOW_DATA_MANIFEST
)

file(
    TO_CMAKE_PATH
    ${CMAKE_CURRENT_BINARY_DIR}/Config
    AIRGLOW_CONFIG
)

add_subdirectory(libs/Glow)
add_subdirectory(Airglow)
