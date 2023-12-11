cmake_minimum_required(VERSION 3.27)

file(
    READ
    ${CMAKE_SOURCE_DIR}/Airglow.json
    AIRGLOW_JSON
)

string(
    JSON
    NAME
    GET
    ${AIRGLOW_JSON}
    NAME
)

string(
    JSON
    VERSION
    GET
    ${AIRGLOW_JSON}
    VERSION
)

project(
    ${NAME}
    VERSION ${VERSION}
    LANGUAGES C
              CXX
)

add_subdirectory(libs/Glow)

if(AIRGLOW_NEXT)
    add_subdirectory(next)
else()
    add_subdirectory(previous)
endif()
