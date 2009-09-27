for %%i in (D:\Projects\GenericHID\generichid\help\*.htm) do D:\Projects\GenericHID\makehelp\debug\makehelp D:\SOURCE\Inetpub\wwwroot\CNC\GenericHID "%%i"
if ERRORLEVEL 1 goto error
for %%i in (D:\Projects\GenericHID\generichid\help\*.png) do copy "%%i" D:\SOURCE\Inetpub\wwwroot\CNC\GenericHID
if ERRORLEVEL 1 goto error
for %%i in (D:\Projects\GenericHID\generichid\help\*.jpg) do copy "%%i" D:\SOURCE\Inetpub\wwwroot\CNC\GenericHID
if ERRORLEVEL 1 goto error
:done
exit /B
:error
echo "Fatal error"
