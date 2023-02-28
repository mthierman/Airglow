function Initialize-DevShell64 {
    $vswhere = "${Env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $vspath = & $vswhere -products * -latest -property installationPath
    & "$vspath\Common7\Tools\Launch-VsDevShell.ps1" -HostArch amd64 -Arch amd64 -SkipAutomaticLocation
}
Set-Alias -Name dev -Value Initialize-DevShell64

function Initialize-DevShell32 {
    $vswhere = "${Env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $vspath = & $vswhere -products * -latest -property installationPath
    & "$vspath\Common7\Tools\Launch-VsDevShell.ps1" -HostArch x86 -Arch x86 -SkipAutomaticLocation
}
Set-Alias -Name dev32 -Value Initialize-DevShell32

function Install-WebView2 {
    nuget install Microsoft.Web.WebView2 -OutputDirectory packages
}
Set-Alias -Name webview2 -Value Install-WebView2

function Install-CppWinRT {
    nuget install Microsoft.Windows.CppWinRT -OutputDirectory packages
}
Set-Alias -Name cppwinrt -Value Install-CppWinRT

function Install-ImplementationLibrary {
    nuget install Microsoft.Windows.ImplementationLibrary -OutputDirectory packages
}
Set-Alias -Name wil -Value Install-ImplementationLibrary

function Restore-Packages {
    msbuild -t:restore -p:RestorePackagesConfig=true
}
Set-Alias -Name restore -Value Restore-Packages

function Build-WebView2Debug {
    msbuild Gooey.vcxproj -property:Configuration=Debug -property:Platform=x64
}
Set-Alias -Name debug -Value Build-WebView2Debug

function Build-WebView2Release {
    msbuild Gooey.vcxproj -property:Configuration=Release -property:Platform=x64
}
Set-Alias -Name release -Value Build-WebView2Release

function Build-ReleaseNotes {
    New-Item -Path notes.md
    Write-Output "Changes:`n" | Add-Content -Path notes.md
    git --no-pager log -5 --oneline --no-decorate | Add-Content -Path notes.md
}
Set-Alias -Name notes -Value Build-ReleaseNotes

function Copy-Release {
    $output = "C:\Programs\Gooey"
    if (!(Test-Path $output)) { New-Item -ItemType Directory $output }
    Copy-Item "x64\Release\Gooey.exe" -Destination "$output\Gooey.exe" -Force
}
Set-Alias -Name export -Value Copy-Release
