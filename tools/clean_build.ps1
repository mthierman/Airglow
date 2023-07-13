$repo = $PSScriptRoot | Split-Path

if (Test-Path "$repo/build") { Remove-Item "$repo/build" -Force -Recurse }
