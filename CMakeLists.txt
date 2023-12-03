cmake_minimum_required(VERSION 3.27)
include(FetchContent)

list(
    APPEND
    CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(PackageInfo)

project(
    ${NAME}
    VERSION ${VERSION}
    LANGUAGES C
              CXX
)

include(Data)

add_subdirectory(libs/Glow)

add_subdirectory(Airglow)
