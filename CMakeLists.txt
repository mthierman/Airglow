cmake_minimum_required(VERSION 3.27)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/ReadJSON.cmake)

project(
    ${NAME}
    VERSION ${VERSION}
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
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Airglow.manifest
    GLOW_DATA_MANIFEST
)

file(
    TO_CMAKE_PATH
    ${CMAKE_CURRENT_BINARY_DIR}/Config
    AIRGLOW_CONFIG
)

add_subdirectory(libs/Glow)
add_subdirectory(Airglow)
