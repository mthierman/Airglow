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

function Build-Debug {
    cmake --no-warn-unused-cli -Bbuild\Debug -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++"
    cmake --build build\Debug
}
Set-Alias -Name debug -Value Build-Debug

function Build-Release {
    cmake --no-warn-unused-cli -Bbuild\Release -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE="Release" -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++"
    cmake --build build\Release
}
Set-Alias -Name release -Value Build-Release

function Build-Preset {
    cmake --preset $args
    cmake --build --preset $args
}
Set-Alias -Name preset -Value Build-WebView2Preset

function Build-ReleaseNotes {
    New-Item -Path notes.md
    Write-Output "Changes:`n" | Add-Content -Path notes.md
    git --no-pager log -5 --oneline --no-decorate | Add-Content -Path notes.md
}
Set-Alias -Name notes -Value Build-ReleaseNotes

function Export-Release {
    $build = Split-Path $MyInvocation.PSScriptRoot | Join-Path -ChildPath "build" -AdditionalChildPath "Release"
    $programs = "C:\Programs\Gooey"
    if (!(Test-Path $programs)) { New-Item -ItemType Directory $programs }
    Copy-Item "$build\Gooey.exe" -Destination "$programs\Gooey.exe" -Force
}
Set-Alias -Name export -Value Export-Release

function Build-Libs {
    $CPPWINRT_VER = "2.0.230225.1"
    $WIL_VER = "1.0.230202.1"
    $WEBVIEW_VER = "1.0.1587.40"

    $cppwinrt_params = @{
        "-ChildPath"           = "packages"
        "-AdditionalChildPath" = "Microsoft.Windows.CppWinRT.$CPPWINRT_VER", "bin", "cppwinrt.exe"
    }

    $wil_params = @{
        "-ChildPath"           = "packages"
        "-AdditionalChildPath" = "Microsoft.Windows.ImplementationLibrary.$WIL_VER", "include", "wil"
    }

    $webview_params = @{
        "-ChildPath"           = "packages"
        "-AdditionalChildPath" = "Microsoft.Web.WebView2.$WEBVIEW_VER", "build", "native", "include"
    }

    $loader_params = @{
        "-ChildPath"           = "packages"
        "-AdditionalChildPath" = "Microsoft.Web.WebView2.$WEBVIEW_VER", "build", "native", "x64", "WebView2LoaderStatic.lib"
    }

    $libs_params = @{
        "-ChildPath"           = "build"
        "-AdditionalChildPath" = "libs"
    }

    $cppwinrt = Split-Path $MyInvocation.PSScriptRoot | Join-Path @cppwinrt_params
    $wil = Split-Path $MyInvocation.PSScriptRoot | Join-Path @wil_params
    $webview = Split-Path $MyInvocation.PSScriptRoot | Join-Path @webview_params
    $loader = Split-Path $MyInvocation.PSScriptRoot | Join-Path @loader_params

    $libs = Split-Path $MyInvocation.PSScriptRoot | Join-Path @libs_params
    
    if (Test-Path $libs) { Remove-Item $libs -Recurse }

    & $cppwinrt -input sdk -output $libs
    Copy-Item -Path $wil -Destination $libs -Recurse
    Copy-Item -Path $webview -Destination $libs/webview2 -Recurse
    Copy-Item -Path $loader -Destination $libs
}
Set-Alias -Name libs -Value Build-Libs
