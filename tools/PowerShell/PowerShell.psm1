function Compress-Airglow
{
    Push-Location
    Set-Location "build/Release"
    if (Test-Path "Airglow.zip") { Remove-Item "Airglow.zip" -Force }
    7z a Airglow.zip Airglow.exe gui
    Pop-Location
}

function Restore-Wix
{
    dotnet tool restore
    dotnet wix extension add WixToolset.UI.wixext/4.0.5
    dotnet wix extension add WixToolset.Bal.wixext/4.0.5
}

function Build-AirglowInstaller
{
    dotnet wix build tools\installer\msi.wxs -o build/Airglow.msi -ext WixToolset.UI.wixext
    dotnet wix build tools\installer\bundle.wxs -o build/Airglow.exe -ext WixToolset.Bal.wixext
}

function Publish-Airglow
{
    [CmdletBinding()]
    param (
        [ValidateNotNullOrEmpty()]
        [string]$Branch = 'main'
    )

    # $hash = Get-Content "build/notes/short_hash"
    $version = Get-Content "build/notes/version"
    $notes = Get-Item "build/notes/release_notes"
    $archive = Get-Item "build/Release/Airglow.zip"
    $installer = Get-Item "build/Airglow.exe"

    gh release delete $version -y
    gh release create $version $archive $installer --notes-file $notes -t $version
}

function Get-VCRedist
{
    Push-Location
    Set-Location "build"
    Invoke-WebRequest "https://aka.ms/vs/17/release/vc_redist.x64.exe" -OutFile "vc_redist.x64.exe"
    Pop-Location
}

function Build-AirglowServer
{
    go build -o "./build/Release/AirglowServer.exe" "./tools/server"
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
