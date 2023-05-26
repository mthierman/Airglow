$vswhere = "${Env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$vspath = & $vswhere -products * -latest -property installationPath
& "$vspath\Common7\Tools\Launch-VsDevShell.ps1" -HostArch x86 -Arch x86 -SkipAutomaticLocation
