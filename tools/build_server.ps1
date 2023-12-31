$repo = $PSScriptRoot | Split-Path
go build -o "$repo/build/server.exe" "./server"
