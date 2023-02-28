$build = Split-Path $PSScriptRoot | Split-Path | Join-Path -ChildPath "build" -AdditionalChildPath "Release"
Write-Host $build
