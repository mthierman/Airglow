$repo = $PSScriptRoot | Split-Path
Push-Location
Set-Location "$repo"
if (Test-Path "build") { Remove-Item "build" -Force -Recurse }
Set-Location "$env:LOCALAPPDATA/Airglow/"
Get-ChildItem
if (Test-Path "EBWebView") { Remove-Item "EBWebView" -Force -Recurse }
if (Test-Path "gui") { Remove-Item "gui" -Force -Recurse }
if (Test-Path "Airglow.exe") { Remove-Item "Airglow.exe" -Force -Recurse }
if (Test-Path "Config.json") { Remove-Item "Config.json" -Force -Recurse }
if (Test-Path "Database.sqlite") { Remove-Item "Database.sqlite" -Force -Recurse }
Pop-Location
