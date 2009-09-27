if exist %TEMP%\generichid del /f/s/q %TEMP%\generichid
if exist ..\..\generichid.zip del ..\..\generichid.zip
mkdir %TEMP%\generichid 
mkdir %TEMP%\generichid\help
mkdir %TEMP%\generichid\resources
copy ..\generichid\release\generichid.exe %TEMP%\generichid
copy ..\bin\Joystick.hex %TEMP%\generichid
copy "C:\program files\Microsoft Visual Studio 8\VC\redist\x86\Microsoft.VC80.CRT\Microsoft.VC80.CRT.manifest" %TEMP%\generichid
copy ..\generichid\config.xml %TEMP%\generichid
copy ..\generichid\help %TEMP%\generichid\help
copy ..\generichid\resources %TEMP%\generichid\resources
"c:\program files\7-zip\7z" a -tzip -mx9 ..\..\generichid.zip %TEMP%\generichid\*