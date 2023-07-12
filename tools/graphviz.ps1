$repo = $PSScriptRoot | Split-Path
dot -Tsvg "$repo/build/CMakeFiles/graphviz/graphviz.dot" > "$repo/build/CMakeFiles/graphviz/graphviz.svg"
