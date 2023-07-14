$repo = $PSScriptRoot | Split-Path

if (Test-Path "$repo/build") { Remove-Item "$repo/build" -Force -Recurse }
if (Test-Path "$repo/settings/dist") { Remove-Item "$repo/settings/dist" -Force -Recurse }
