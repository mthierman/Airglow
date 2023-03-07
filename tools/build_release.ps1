Import-Module $PSScriptRoot\dev
gen_release && build_release && export && remove
