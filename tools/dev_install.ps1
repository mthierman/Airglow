$repo = Convert-Path $PSScriptRoot | Split-Path

Get-Content -Raw "$repo/Airglow.json" | ConvertFrom-Json

if (!(Test-Path "$env:LOCALAPPDATA/Airglow")) { New-Item -ItemType Directory "$env:LOCALAPPDATA/Airglow" }
$appdata = "$env:LOCALAPPDATA/Airglow" | Resolve-Path
Write-Host -ForegroundColor Cyan "Installation path:"
Write-Host -ForegroundColor Green $appdata.Path`n

if (Test-Path "$repo/build/Airglow.exe")
{
    $exe = "$repo/build/Airglow.exe" | Resolve-Path
    Write-Host -ForegroundColor Cyan "Built executable found:"
    Write-Host -ForegroundColor Green $exe.Path`n
    Copy-Item -Path $exe -Destination "$appdata/Airglow.exe" -Force
}

if (Test-Path "$repo/gui/dist")
{
    if (Test-Path "$appdata/gui") { Remove-Item "$appdata/gui" -Force -Recurse }
    $gui = "$repo/gui/dist" | Resolve-Path
    Write-Host -ForegroundColor Cyan "GUI folder found:"
    Write-Host -ForegroundColor Green $gui.Path`n
    Copy-Item -Path $gui -Destination $appdata -Recurse -Force
    Rename-Item -Path "$appdata/dist" -NewName "gui" -Force
}

Write-Host -ForegroundColor Blue "Open Airglow?"

$open = Read-Host -Prompt "[y/n]"
if ($open -eq 'y')
{
    Write-Host -ForegroundColor Green "Launching..."`n
    & "$appdata/Airglow.exe"
}

Write-Host -ForegroundColor Green `n"Thanks! Goodbye!"
