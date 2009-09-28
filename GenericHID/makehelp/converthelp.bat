for %%i in (D:\Projects\GenericHID\generichid\help\*.htm) do (
D:\Projects\GenericHID\makehelp\release\makehelp D:\SOURCE\Inetpub\generichidsf "%%i"
if ERRORLEVEL 1 goto error
)

for %%i in (D:\Projects\GenericHID\generichid\help\*.png) do (
copy "%%i" D:\SOURCE\Inetpub\generichidsf
if ERRORLEVEL 1 goto error
)

for %%i in (D:\Projects\GenericHID\generichid\help\*.jpg) do (
copy "%%i" D:\SOURCE\Inetpub\generichidsf
if ERRORLEVEL 1 goto error
)

:done
exit /B
:error
echo "Fatal error"
