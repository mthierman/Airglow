$repo = $PSScriptRoot | Split-Path
Push-Location
if (!(Test-Path "$repo/build"))
{
    New-Item -ItemType Directory $repo/build
}
$changes = New-Item -Path $repo/build/notes.txt
git --no-pager log -5 --oneline --no-decorate | Add-Content -Path $changes
Pop-Location
