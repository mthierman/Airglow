$repo = $PSScriptRoot | Split-Path
Push-Location
Set-Location "$repo/build/graphviz"
dot -Tsvg graphviz.dot > graphviz.svg
Pop-Location
