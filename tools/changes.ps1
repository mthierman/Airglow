if (!(Test-Path "build")) {
    New-Item -ItemType Directory build
}
$changes = New-Item -Path build/changes.md
Write-Output "Changes:`n" | Add-Content -Path $changes
git --no-pager log -5 --oneline --no-decorate | Add-Content -Path $changes
