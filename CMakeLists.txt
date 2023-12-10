cmake_minimum_required(VERSION 3.27)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/ReadJSON.cmake)

project(
    ${NAME}
    VERSION ${VERSION}
    LANGUAGES C
              CXX
)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Configure.cmake)

add_subdirectory(libs/Glow)
add_subdirectory(Airglow)
