function ConvertTo-WixIco
{
    param (
        $in,
        $out
    )
    magick convert -background none $in -define icon:auto-resize="16" $out
}

ConvertTo-WixIco WixUINewIco.svg WixUINewIco.ico
ConvertTo-WixIco WixUIUpIco.svg WixUIUpIco.ico
