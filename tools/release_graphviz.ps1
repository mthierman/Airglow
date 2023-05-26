Push-Location
cmake --preset Release --graphviz=build/graphviz.dot
Set-Location "build"
dot -Tsvg graphviz.dot > graphviz.svg
Pop-Location
