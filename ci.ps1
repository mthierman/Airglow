& .\pwsh.ps1

Invoke-DevShell
Invoke-CMake -ConfigurePreset 'CI' -BuildPreset 'CI'
Compress-Airglow
Restore-Wix
Build-AirglowInstaller
# Publish-Airglow
