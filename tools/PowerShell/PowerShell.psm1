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
    go build -o "./build/Release/AirglowServer.exe" "./tools/server"
}

function Invoke-Archive
{
    Push-Location
    Set-Location build/Release
    7z a Airglow.zip Airglow.exe gui
    Pop-Location
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

function Get-Version
{
    Get-Content build/Release/notes/version
}

function Get-ShortHash
{
    Get-Content build/Release/notes/short_hash
}

function Get-ReleaseNotes
{
    Get-Item build/Release/notes/release_notes
}

function Get-Archive
{
    Get-Item build/Release/Airglow.zip
}

function Invoke-StableRelease
{
    $version = Get-Version
    $notes = Get-ReleaseNotes
    $archive = Get-Archive

    gh release delete $version -y
    gh release create $version $archive --notes-file $notes -t $version
}

function Invoke-DevRelease
{
    $hash = Get-ShortHash
    $notes = Get-ReleaseNotes
    $archive = Get-Archive
    
    gh release delete $hash -y
    gh release create $hash $archive --notes-file $notes -t $hash -p
}
