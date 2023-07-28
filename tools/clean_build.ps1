$repo = $PSScriptRoot | Split-Path

if (Test-Path "$repo/build") { Remove-Item "$repo/build" -Force -Recurse }
if (Test-Path "$repo/gui/dist") { Remove-Item "$repo/gui/dist" -Force -Recurse }
