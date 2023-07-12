$root = $PSScriptRoot

if (!(Test-Path "$env:LOCALAPPDATA/Airglow")) 
{
    New-Item -ItemType Directory "$env:LOCALAPPDATA/Airglow" 
}
$destination = "$env:LOCALAPPDATA/Airglow" | Resolve-Path

Get-Content -Raw "$root/Airglow.json" | ConvertFrom-Json -AsHashtable | Format-Table -AutoSize -HideTableHeaders

Write-Host -ForegroundColor Cyan "Installing to `"$($destination.Path)\Airglow.exe`"...`n"

Copy-Item -Path "$root/Airglow.exe" -Destination "$destination/Airglow.exe"
Copy-Item -Path "$root/gui" -Destination "$destination/gui" -Recurse -Force
Copy-Item -Path "$root/Airglow.json" -Destination "$destination/Airglow.json"
Copy-Item -Path "$root/Airglow.js" -Destination "$destination/Airglow.js"

$open = Read-Host -Prompt "$($PSStyle.Foreground.Cyan)Open Airglow? $($PSStyle.Reset)[y/n]"
if ($open -eq 'y')
{
    & "$destination/Airglow.exe"
}

Write-Host -ForegroundColor Green "`n🌃 Installation Successful`n"
