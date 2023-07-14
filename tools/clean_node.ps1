$repo = $PSScriptRoot | Split-Path

if (Test-Path "$repo/settings/node_modules") { Remove-Item "$repo/settings/node_modules" -Force -Recurse }
if (Test-Path "$repo/settings/pnpm-lock.yaml") { Remove-Item "$repo/settings/pnpm-lock.yaml" -Force -Recurse }
