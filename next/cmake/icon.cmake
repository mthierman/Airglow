file(
    TO_CMAKE_PATH
    ${PROJECT_BINARY_DIR}/data/Glow.rc
    GLOW_DATA_RC
)

file(
    GENERATE
    OUTPUT ${GLOW_DATA_RC}
    CONTENT "1 ICON \"Glow.ico\""
)

# set(GLOW_ICON_SOURCE
#     ${PROJECT_SOURCE_DIR}/data/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>.ico
# )

# set(GLOW_ICON
#     ${PROJECT_BINARY_DIR}/data/Glow.ico
# )

# add_custom_command(
#     OUTPUT ${GLOW_ICON}
#     COMMAND ${CMAKE_COMMAND} -E copy ${GLOW_ICON_SOURCE} ${GLOW_ICON}
#     COMMENT "Copying icon..."
# )

# add_custom_target(
#     MakeIcon
#     ALL
#     DEPENDS ${GLOW_ICON}
# )
