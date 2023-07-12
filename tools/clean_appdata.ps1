$appdata = "$env:LOCALAPPDATA/Airglow"

if (Test-Path "$appdata/Airglow.exe") { Remove-Item "$appdata/Airglow.exe" -Force -Recurse }
if (Test-Path "$appdata/gui") { Remove-Item "$appdata/gui" -Force -Recurse }
if (Test-Path "$appdata/Airglow.json") { Remove-Item "$appdata/Airglow.json" -Force -Recurse }
if (Test-Path "$appdata/Airglow.js") { Remove-Item "$appdata/Airglow.js" -Force -Recurse }

if (Test-Path "$appdata/EBWebView") { Remove-Item "$appdata/EBWebView" -Force -Recurse }
if (Test-Path "$appdata/Config.json") { Remove-Item "$appdata/Config.json" -Force -Recurse }
if (Test-Path "$appdata/Database.sqlite") { Remove-Item "$appdata/Database.sqlite" -Force -Recurse }
