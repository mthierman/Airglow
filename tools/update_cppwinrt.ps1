$repo = $PSScriptRoot | Split-Path
nuget install Microsoft.Windows.CppWinRT -OutputDirectory $repo/external
