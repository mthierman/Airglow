# pwsh -nop -nol -ExecutionPolicy Bypass -File .\install.ps1
# irm https://github.com/mthierman/Airglow/releases/download/stable/install.ps1 | iex

$download = 'https://github.com/mthierman/Airglow/releases/download/stable/Airglow.zip'
$T = "$($env:TEMP)\tmp$([System.Convert]::ToString((Get-Random 65535),16).padleft(4, '0')).tmp"
$temp = ((New-Item -ItemType Directory -Path $T) | Resolve-Path).ProviderPath

Invoke-WebRequest -Uri $download -OutFile "$temp/Airglow.zip"
$zip = ("$temp/Airglow.zip" | Resolve-Path).ProviderPath
[System.IO.Compression.ZipFile]::ExtractToDirectory($zip, "$temp/Airglow")

$bundle = ("$temp/Airglow" | Resolve-Path).ProviderPath

Copy-Item -Path $bundle -Destination $env:LOCALAPPDATA -Recurse -Force
Remove-Item $temp -Recurse -Force

$shell = New-Object -ComObject WScript.Shell
$exe = ("$env:LOCALAPPDATA/Airglow/Airglow.exe" | Resolve-Path).ProviderPath
$startmenu = ([System.Environment]::GetFolderPath("StartMenu") | Join-Path -ChildPath "Programs" | Resolve-Path).ProviderPath
$shortcut = $shell.CreateShortcut("$startmenu/Airglow.lnk")
$shortcut.TargetPath = $exe
$shortcut.Save()

Write-Host -ForegroundColor Green "`nInstallation Successful`n"
$open = Read-Host -Prompt "$($PSStyle.Foreground.Cyan)Open Airglow? [y/n]"
if ($open -eq 'y')
{
    $exe = ("$env:LOCALAPPDATA/Airglow/Airglow.exe" | Resolve-Path).ProviderPath
    & $exe
}
