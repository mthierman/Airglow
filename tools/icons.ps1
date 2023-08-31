$repo = $PSScriptRoot | Split-Path

magick convert -background none "$repo/data/icons/icon.svg" -define icon:auto-resize="256,64,48,32,24,16" "$repo/data/icons/icon.ico"
magick convert -background none "$repo/data/icons/debug.svg" -define icon:auto-resize="256,64,48,32,24,16" "$repo/data/icons/debug.ico"
