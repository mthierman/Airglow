cmake_minimum_required(VERSION 3.27)

add_subdirectory(libs/Glow)

project(
    Airglow
    VERSION 1.6.0
    LANGUAGES C
              CXX
)

add_subdirectory(Airglow)
