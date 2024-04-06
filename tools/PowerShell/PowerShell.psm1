function Compress-Airglow
{
    [CmdletBinding()]
    param ()

    if (Test-Path "build/Airglow.zip") { Remove-Item "build/Airglow.zip" -Force }
    Push-Location
    Set-Location "build/Release"
    7z a ../Airglow.zip Airglow.exe gui
    Pop-Location
}

function Restore-Wix
{
    dotnet tool restore
    wix extension add WixToolset.UI.wixext/5.0.0
    wix extension add WixToolset.BootstrapperApplications.wixext/5.0.0
}

function Build-AirglowInstaller
{
    wix build tools\installer\msi.wxs -o build/Airglow.msi -ext WixToolset.UI.wixext
    wix build tools\installer\bundle.wxs -o build/Airglow.exe -ext WixToolset.BootstrapperApplications.wixext
}

function Publish-Airglow
{
    [CmdletBinding()]
    param (
        [ValidateNotNullOrEmpty()]
        [string]$Branch = 'main'
    )

    # $hash = Get-Content "build/notes/short_hash"
    # $version = Get-Content "build/notes/version"
    # $notes = Get-Item "build/notes/release_notes"

    $version = "v$((Get-Content "build/manifest.json" | ConvertFrom-Json).version)"
    $notes = Get-Item "build/notes/release_notes"
    $archive = Get-Item "build/Airglow.zip"
    $installer = Get-Item "build/Airglow.exe"

    gh release delete $version -y
    gh release create $version $archive $installer --notes-file $notes -t $version
}

function Get-VCRedist
{
    if (Test-Path "build/vc_redist.x64.exe") { Remove-Item "build/vc_redist.x64.exe" -Force }
    Invoke-WebRequest "https://aka.ms/vs/17/release/vc_redist.x64.exe" -OutFile "build/vc_redist.x64.exe"
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
