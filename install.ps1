# wix extension add WixToolset.UI.wixext -g
wix build .\install.wxs -o build/Airglow.msi -ext WixToolset.UI.wixext
wix build .\bundle.wxs -o build/Bundle.exe -ext WixToolset.Bal.wixext

# .\VC_redist.x64.exe /install /passive /norestart
# .\VC_redist.x64.exe /install /quiet
# .\VC_redist.x64.exe /install
# .\VC_redist.x64.exe /quiet
