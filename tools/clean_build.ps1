$repo = $PSScriptRoot | Split-Path

if (Test-Path "$repo/build-Debug") { Remove-Item "$repo/build-Debug" -Force -Recurse }
if (Test-Path "$repo/gui/dist") { Remove-Item "$repo/gui/dist" -Force -Recurse }
