file(
    READ
    ${CMAKE_SOURCE_DIR}/Airglow.json
    PROJECT_JSON
)

string(
    JSON
    NAME
    GET
    ${PROJECT_JSON}
    NAME
)

string(
    JSON
    VERSION
    GET
    ${PROJECT_JSON}
    VERSION
)

file(
    READ
    ${CMAKE_SOURCE_DIR}/libs/Glow/Glow.json
    GLOW_JSON
)

string(
    JSON
    WEBVIEW
    GET
    ${GLOW_JSON}
    WEBVIEW
)

string(
    JSON
    WEBVIEW_VER
    GET
    ${GLOW_JSON}
    WEBVIEW_VER
)
