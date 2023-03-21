nuget install Microsoft.Web.WebView2 -OutputDirectory external
nuget install Microsoft.Windows.ImplementationLibrary -OutputDirectory external
cmake --preset Release
cmake --build --preset Release
(Get-Date | Out-String).trim() > build/Release/date.txt
git --no-pager log -5 --oneline --no-decorate >> build/Release/notes.txt
