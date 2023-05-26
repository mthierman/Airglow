Push-Location
cmake --preset Debug --graphviz=build/graphviz.dot
Set-Location "build"
dot -Tsvg graphviz.dot > graphviz.svg
Pop-Location
