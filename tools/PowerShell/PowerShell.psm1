function Invoke-Go
{
    go build -o "./build/Release/AirglowServer.exe" "./tools/server"
}

function Add-WixExtensions
{
    dotnet tool restore
    dotnet wix extension add WixToolset.UI.wixext
    dotnet wix extension add WixToolset.Bal.wixext
}

function Invoke-Archive
{
    Push-Location
    Set-Location build/Release
    if (Test-Path Airglow.zip) { Remove-Item Airglow.zip -Force }
    7z a Airglow.zip Airglow.exe gui
    Pop-Location
}

function Invoke-Wix
{
    dotnet tool restore
    dotnet wix build tools\installer\msi.wxs -o build/Airglow.msi -ext WixToolset.UI.wixext
    dotnet wix build tools\installer\bundle.wxs -o build/Airglow.exe -ext WixToolset.Bal.wixext
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

function Get-Installer
{
    Get-Item build/Airglow.exe
}

function Publish-Airglow
{
    [CmdletBinding()]
    param (
        [ValidateNotNullOrEmpty()]
        [string]$Branch = 'main'
    )

    $hash = Get-ShortHash
    $version = Get-Version
    $notes = Get-ReleaseNotes
    $archive = Get-Archive
    $installer = Get-Installer

    gh release delete $version -y
    gh release create $version $archive $installer --notes-file $notes -t $version
}
