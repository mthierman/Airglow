$global:changed = $false
$folder = Split-Path $PSScriptRoot
$filter = "CMakeLists.txt"
$notify = [IO.NotifyFilters]::LastWrite
$change = [IO.WatcherChangeTypes]::Changed
$timeout = 1

try {
    Write-Warning "FileSystemWatcher is monitoring $path"
    $watcher = New-Object -TypeName IO.FileSystemWatcher -ArgumentList $folder, $filter -Property @{
        IncludeSubdirectories = $false
        NotifyFilter          = $notify
        EnableRaisingEvents   = $true
    }
    Register-ObjectEvent $watcher "Changed" -Action { $global:changed = $true }

    do {
        while ($global:changed -eq $false) {
            Start-Sleep -Milliseconds 1
        }
        $result = $watcher.WaitForChanged($change, $timeout)
        if ($result.TimedOut) { continue }
        # if ($watcher.WaitForChanged($change, $timeout).TimedOut) { continue }
        Write-Host "Tada"
        $global:changed = $false
    } while ($true)
}
finally {
    # Remove-TypeData IO.FileSystemWatcher
    $watcher.Dispose()
    Write-Warning "FileSystemWatcher Removed"
    # Remove-TypeData IO.FileSystemWatcher
}
