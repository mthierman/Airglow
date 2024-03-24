function ConvertTo-Ico
{
    param (
        $in,
        $out
    )
    magick convert -background none $in -define icon:auto-resize="256,128,96,80,72,64,60,48,40,36,32,30,24,20,16" $out
}

ConvertTo-Ico release.svg release.ico
ConvertTo-Ico debug.svg debug.ico
