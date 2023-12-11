cmake_minimum_required(VERSION 3.27)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/ReadJSON.cmake)

project(
    ${NAME}
    VERSION ${VERSION}
    LANGUAGES C
              CXX
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/Airglow_artifacts
)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Configure.cmake)

add_subdirectory(libs/Glow)

if(AIRGLOW_NEXT)
    add_subdirectory(next)
else()
    add_subdirectory(previous)
endif()
