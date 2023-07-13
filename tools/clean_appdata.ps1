$appdata = "$env:LOCALAPPDATA/Airglow"

if (Test-Path "$appdata") { Remove-Item "$appdata/*" -Force -Recurse }
