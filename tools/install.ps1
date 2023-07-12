$root = $PSScriptRoot

if (!(Test-Path "$env:LOCALAPPDATA/Airglow")) 
{
    New-Item -ItemType Directory "$env:LOCALAPPDATA/Airglow" 
}
$destination = "$env:LOCALAPPDATA/Airglow" | Resolve-Path

Get-Content -Raw "$root/Airglow.json" | ConvertFrom-Json

Write-Host -ForegroundColor Cyan "Installing to `"$($destination.Path)\Airglow.exe`"..."`n

Copy-Item -Path "$root/Airglow.exe" -Destination "$destination/Airglow.exe"
Copy-Item -Path "$root/gui" -Destination "$destination/gui" -Recurse

$open = Read-Host -Prompt "$($PSStyle.Foreground.Green)Open Airglow? $($PSStyle.Foreground.Cyan)[y/n]"
if ($open -eq 'y')
{
    Write-Host -ForegroundColor Green "Launching..."`n
    & "$destination/Airglow.exe"
}

Write-Host -ForegroundColor Green "🌃 Installation Successful"
