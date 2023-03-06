# $watcher = New-Object System.IO.FileSystemWatcher
# $watcher.Path = Get-Location
# $watcher.IncludeSubdirectories = $true
# $watcher.EnableRaisingEvents = $false
# $watcher.NotifyFilter = [System.IO.NotifyFilters]::LastWrite

# $watcher.WaitForChanged([System.IO.WatcherChangeTypes]::Changed, 1000)
