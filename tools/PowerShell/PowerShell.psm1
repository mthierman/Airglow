function Invoke-DevShell
{
    [CmdletBinding()]
    param (
        [ValidateNotNullOrEmpty()]
        [string]$Arch = 'amd64'
    )

    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $vspath = & $vswhere -products * -latest -property installationPath
    & "$vspath\Common7\Tools\Launch-VsDevShell.ps1" -HostArch $Arch -Arch $Arch -SkipAutomaticLocation
}

function Invoke-CMake
{
    [CmdletBinding()]
    param (
        [ValidateNotNullOrEmpty()]
        [string]$Preset = 'Unity'
    )

    $Repo = $PSScriptRoot | Split-Path | Split-Path
    Push-Location
    Set-Location $Repo
    Invoke-DevShell
    cmake --preset $Preset
    cmake --build --preset $Preset
    Pop-Location
}

function Invoke-Go
{
    $Repo = $PSScriptRoot | Split-Path | Split-Path
    Push-Location
    Set-Location $Repo
    go build -o "./build/Release/AirglowServer.exe" "./tools/server"
    Pop-Location
}

function Invoke-Archive
{
    $Repo = $PSScriptRoot | Split-Path | Split-Path
    Push-Location
    Set-Location $Repo/build/Release
    if (Test-Path Airglow.zip) { Remove-Item Airglow.zip -Force }
    7z a Airglow.zip Airglow.exe gui
    Pop-Location
}

function Add-WixExtensions
{
    $Repo = $PSScriptRoot | Split-Path | Split-Path
    Push-Location
    Set-Location $Repo
    dotnet tool restore
    dotnet wix extension add WixToolset.UI.wixext
    dotnet wix extension add WixToolset.Bal.wixext
    Pop-Location
}

function Invoke-Wix
{
    $Repo = $PSScriptRoot | Split-Path | Split-Path
    Push-Location
    Set-Location $Repo
    dotnet tool restore
    dotnet wix build tools\installer\msi.wxs -o build/Airglow.msi -ext WixToolset.UI.wixext
    dotnet wix build tools\installer\bundle.wxs -o build/Airglow.exe -ext WixToolset.Bal.wixext
    Pop-Location
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

function Get-Version
{
    $Repo = $PSScriptRoot | Split-Path | Split-Path
    Push-Location
    Set-Location $Repo
    Get-Content build/Release/notes/version
    Pop-Location
}

function Get-ShortHash
{
    $Repo = $PSScriptRoot | Split-Path | Split-Path
    Push-Location
    Set-Location $Repo
    Get-Content build/Release/notes/short_hash
    Pop-Location
}

function Get-ReleaseNotes
{
    $Repo = $PSScriptRoot | Split-Path | Split-Path
    Push-Location
    Set-Location $Repo
    Get-Item build/Release/notes/release_notes
    Pop-Location
}

function Get-Archive
{
    $Repo = $PSScriptRoot | Split-Path | Split-Path
    Push-Location
    Set-Location $Repo
    Get-Item build/Release/Airglow.zip
    Pop-Location
}

function Get-Installer
{
    $Repo = $PSScriptRoot | Split-Path | Split-Path
    Push-Location
    Set-Location $Repo
    Get-Item build/Airglow.exe
    Pop-Location
}

function Invoke-StableRelease
{
    $Repo = $PSScriptRoot | Split-Path | Split-Path
    Push-Location
    Set-Location $Repo

    $version = Get-Version
    $notes = Get-ReleaseNotes
    $archive = Get-Archive

    gh release delete $version -y
    gh release create $version $archive --notes-file $notes -t $version

    Pop-Location
}

function Invoke-DevRelease
{
    $Repo = $PSScriptRoot | Split-Path | Split-Path
    Push-Location
    Set-Location $Repo

    $hash = Get-ShortHash
    $notes = Get-ReleaseNotes
    $archive = Get-Archive

    gh release delete $hash -y
    gh release create $hash $archive --notes-file $notes -t $hash -p

    Pop-Location
}
