$repo = $PSScriptRoot | Split-Path
nuget install Microsoft.Windows.ImplementationLibrary -OutputDirectory $repo/external
