Import-Module $PSScriptRoot\dev
gen_debug && build_debug
Remove-Module dev
