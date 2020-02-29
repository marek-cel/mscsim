call "%WIX%bin\candle.exe" -ext WixBalExtension wix_bundle.wxs
call "%WIX%bin\light.exe" -ext WixBalExtension -out mscsim-0.3-win32.exe wix_bundle.wixobj
