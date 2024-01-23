function Enter-DevShell64
{
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $vspath = & $vswhere -products * -latest -property installationPath
    & "$vspath\Common7\Tools\Launch-VsDevShell.ps1" -HostArch amd64 -Arch amd64 -SkipAutomaticLocation
}

function Enter-DevShell32
{
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $vspath = & $vswhere -products * -latest -property installationPath
    & "$vspath\Common7\Tools\Launch-VsDevShell.ps1" -HostArch x86 -Arch x86 -SkipAutomaticLocation
}

function Invoke-ReleaseBuild
{
    Enter-DevShell64
    cmake --preset Release
    cmake --build --preset Release
}

function Invoke-UnityBuild
{
    Enter-DevShell64
    cmake --preset Unity
    cmake --build --preset Unity
}

function Invoke-ServerBuild
{
    go build -o ".\build\server.exe" ".\tools\server"
}

function Add-WixExtensions
{
    wix extension add WixToolset.UI.wixext
    wix extension add WixToolset.Bal.wixext
}

function Invoke-WixBuild
{
    wix build tools\installer\msi.wxs -o build/Airglow.msi -ext WixToolset.UI.wixext
    wix build tools\installer\bundle.wxs -o build/Airglow.exe -ext WixToolset.Bal.wixext
}

function ConvertTo-WixIco
{
    param (
        $size,
        $in,
        $out
    )
    magick convert -background none $in -define icon:auto-resize="$size" $out
}

function Convert-Icons
{
    ConvertTo-WixIco 16 data/ic_fluent_folder_add_16_regular.svg data/WixUINewIco.ico
    ConvertTo-WixIco 16 data/ic_fluent_folder_arrow_up_16_regular.svg data/WixUIUpIco.ico
    ConvertTo-WixIco 32 data/ic_fluent_error_circle_48_regular.svg data/WixUIExclamationIco.ico
    ConvertTo-WixIco 32 data/ic_fluent_info_48_regular.svg data/WixUIInfoIco.ico
}

function Export-Notes
{
    git --no-pager log -5 --oneline --no-decorate | Out-File "..\notes.txt"
}

function Compress-Repo
{
    Push-Location
    Set-Location ..\
    C:\msys64\usr\bin\bsdtar.exe --exclude-vcs --exclude Glow/build --exclude Glow/.vscode -cJf Glow.tar.xz Glow
    Pop-Location
}

function Get-Commit
{
    git rev-parse --short HEAD
}

function Get-Name
{
    (Get-Content .\Glow.json | ConvertFrom-Json).name
}

function Get-Version
{
    (Get-Content .\Glow.json | ConvertFrom-Json).version
}

function Get-Archive
{
    Get-Item "..\Glow.tar.xz"
}

function Get-Notes
{
    Get-Item "..\notes.txt"
}

function Invoke-StableRelease
{
    Export-Notes
    Compress-Repo
    $version = "v$(Get-Version)"
    $archive = Get-Archive
    $notes = Get-Notes
    gh release delete $version -y
    gh release create $version $archive -F $notes -t "$version"
}

function Invoke-NextRelease
{
    Export-Notes
    Compress-Repo
    $archive = Get-Archive
    $notes = Get-Notes
    gh release delete Next -y
    gh release create Next $archive -F $notes -t "Next" -p
}
