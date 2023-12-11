cmake_minimum_required(VERSION 3.27)

project(
    Airglow
    VERSION 1.6.0
    LANGUAGES C
              CXX
)

add_subdirectory(libs/Glow)

if(AIRGLOW_NEXT)
    add_subdirectory(next)
else()
    add_subdirectory(previous)
endif()
