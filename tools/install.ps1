wix extension add WixToolset.UI.wixext
wix extension add WixToolset.Bal.wixext
wix build .\msi.wxs -o build/Airglow.msi -ext WixToolset.UI.wixext
wix build .\bundle.wxs -o build/Airglow.exe -ext WixToolset.Bal.wixext
