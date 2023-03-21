# VS

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

# NUGET

function Install-WebView2 {
    nuget install Microsoft.Web.WebView2 -OutputDirectory external
}
Set-Alias -Name webview2 -Value Install-WebView2

function Install-CppWinRT {
    nuget install Microsoft.Windows.CppWinRT -OutputDirectory external
}
Set-Alias -Name cppwinrt -Value Install-CppWinRT

function Install-ImplementationLibrary {
    nuget install Microsoft.Windows.ImplementationLibrary -OutputDirectory external
}
Set-Alias -Name wil -Value Install-ImplementationLibrary

function Build-Libs {
    $WEBVIEW_VER = "1.0.1661.34"
    $CPPWINRT_VER = "2.0.230225.1"
    $WIL_VER = "1.0.230202.1"

    $cppwinrt_params = @{
        "-ChildPath"           = "external"
        "-AdditionalChildPath" = "Microsoft.Windows.CppWinRT.$CPPWINRT_VER", "bin", "cppwinrt.exe"
    }

    $wil_params = @{
        "-ChildPath"           = "external"
        "-AdditionalChildPath" = "Microsoft.Windows.ImplementationLibrary.$WIL_VER", "include", "wil"
    }

    $webview_params = @{
        "-ChildPath"           = "external"
        "-AdditionalChildPath" = "Microsoft.Web.WebView2.$WEBVIEW_VER", "build", "native", "include"
    }

    $loader_params = @{
        "-ChildPath"           = "external"
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

# CMAKE

function Invoke-GenerationGraphViz {
    cmake --preset $args --graphviz=build/graphviz/$args.dot
}

function Build-GraphVizSvg {
    dot -Tsvg $args.dot > $args.svg
}

function Invoke-Generation {
    cmake --preset $args
    Copy-Item "build/$args/compile_commands.json" -Destination "build"
}
Set-Alias -Name generate -Value Invoke-Generation

function Invoke-Build {
    cmake --build --preset $args
}
Set-Alias -Name build -Value Invoke-Build

function Invoke-Preset {
    cmake --preset $args
    cmake --build --preset $args
    # Copy-Item "build/$args/compile_commands.json" -Destination "build"
}
Set-Alias -Name preset -Value Invoke-Preset

function Watch-CMake {
    $folder = Split-Path $MyInvocation.PSScriptRoot
    $filter = "CMakeLists.txt"
    $notify = [IO.NotifyFilters]::LastWrite
    $change = [IO.WatcherChangeTypes]::Changed
    $timeout = 1000

    try {
        Write-Warning "FileSystemWatcher is monitoring $folder\$filter"
        Write-Warning "Running initial generation..."
        generate Debug
        $watcher = New-Object -TypeName IO.FileSystemWatcher -ArgumentList $folder, $filter -Property @{
            IncludeSubdirectories = $false
            NotifyFilter          = $notify
        }
        while ($true) {
            if ($watcher.WaitForChanged($change, $timeout).TimedOut) { continue }
            generate Debug
        }
    }

    finally {
        $watcher.Dispose()
        Write-Warning "FileSystemWatcher Removed"
        remove
    }
}
Set-Alias -Name watch -Value Watch-CMake

# MISC

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

function Remove-DevModule {
    Remove-Module dev
}
Set-Alias -Name remove -Value Remove-DevModule

function New-GitHubRelease {
    cmake --preset Release
    cmake --build --preset Release
    (Get-Date | Out-String).trim() > build/Release/date.txt
    git --no-pager log -5 --oneline --no-decorate > build/Release/changes.txt

    $version = Get-Content "build/Release/version.txt"
    $date = Get-Content "build/Release/date.txt"
    $changes = Get-Item "build/Release/changes.txt"
    $exe = Get-Item "build/Release/Gooey.exe"

    # if ("${{ github.ref_name }}" -eq "main") {
    #     $title = "Gooey $version"
    #     gh release delete $version -y
    #     gh release create $version $exe --notes-file $changes -t $title
    # }
    # if ("${{ github.ref_name }}" -eq "dev") {
    #     $title = "Gooey Nightly"
    #     gh release delete nightly -y
    #     gh release create nightly $exe --notes-file $changes -t $title --prerelease
    # }
}

function Remove-BuildFolder {
    Remove-Item build/ -Recurse -Force
}

function Export-ReleaseBuild {
    Copy-Item 'build/Release/Gooey.exe' -Destination 'C:/programs/Gooey/Gooey.exe' -Force
}

function Install-NugetPackages {
    nuget install Microsoft.Web.WebView2 -OutputDirectory external
    nuget install Microsoft.Windows.ImplementationLibrary -OutputDirectory external
}
