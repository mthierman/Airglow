$repo = $PSScriptRoot | Split-Path

magick convert -background none "$repo/data/icon.svg" -define icon:auto-resize="256,64,48,32,24,16" "$repo/data/icon.ico"
magick convert -background none "$repo/data/debug.svg" -define icon:auto-resize="256,64,48,32,24,16" "$repo/data/debug.ico"
