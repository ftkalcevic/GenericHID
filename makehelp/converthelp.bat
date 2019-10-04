set HOMEDIR=.
for %%i in (%HOMEDIR%\generichid\help\*.htm) do (
%HOMEDIR%\makehelp\release\makehelp D:\SOURCE\Inetpub\generichidsf "%%i"
if ERRORLEVEL 1 goto error
)

for %%i in (%HOMEDIR%\generichid\help\*.png) do (
copy "%%i" D:\SOURCE\Inetpub\generichidsf
if ERRORLEVEL 1 goto error
)

for %%i in (%HOMEDIR%\generichid\help\*.jpg) do (
copy "%%i" D:\SOURCE\Inetpub\generichidsf
if ERRORLEVEL 1 goto error
)

:done
exit /B
:error
echo "Fatal error"
