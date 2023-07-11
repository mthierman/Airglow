$repo = $PSScriptRoot | Split-Path
Push-Location
Set-Location "$repo/data/icons"
magick convert -background none icon.svg -define icon:auto-resize="256,64,48,32,24,16" icon.ico
magick convert -background none debug_icon.svg -define icon:auto-resize="256,64,48,32,24,16" debug_icon.ico
Pop-Location
