$repo = $PSScriptRoot | Split-Path
nuget install Microsoft.Web.WebView2 -OutputDirectory $repo/external
