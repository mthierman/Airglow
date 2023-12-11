add_executable(
    ${PROJECT_NAME}
    WIN32
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE Main.cxx
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE glow::glow
            glow::flags
)
