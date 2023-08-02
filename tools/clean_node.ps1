$repo = $PSScriptRoot | Split-Path

if (Test-Path "$repo/gui/node_modules") { Remove-Item "$repo/gui/node_modules" -Force -Recurse }
if (Test-Path "$repo/gui/pnpm-lock.yaml") { Remove-Item "$repo/gui/pnpm-lock.yaml" -Force -Recurse }
