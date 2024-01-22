function ConvertTo-WixIco16
{
    param (
        $in,
        $out
    )
    magick convert -background none $in -define icon:auto-resize="16" $out
}

function ConvertTo-WixIco32
{
    param (
        $in,
        $out
    )
    magick convert -background none $in -define icon:auto-resize="32" $out
}

ConvertTo-WixIco16 ic_fluent_folder_add_16_regular.svg WixUINewIco.ico
ConvertTo-WixIco16 ic_fluent_folder_arrow_up_16_regular.svg WixUIUpIco.ico
ConvertTo-WixIco32 ic_fluent_error_circle_48_regular.svg WixUIExclamationIco.ico
ConvertTo-WixIco32 ic_fluent_info_48_regular.svg WixUIInfoIco.ico
