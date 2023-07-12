$repo = $PSScriptRoot | Split-Path
dot -Tsvg "$repo/build/graphviz/graphviz.dot" > "$repo/build/graphviz/graphviz.svg"
