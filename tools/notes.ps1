$repo = $PSScriptRoot | Split-Path

if (!(Test-Path "$repo/build"))
{
    New-Item -ItemType Directory "$repo/build"
}

$changes = New-Item -Path "$repo/build/release_notes.txt"
git --no-pager log -5 --oneline --no-decorate | Add-Content -Path $changes
