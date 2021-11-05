:: Check MSYS environment
if not exist "%EnvMSYS2BinPath%" %cecho% error "Please install MSYS2 first." & exit /B 1

"%EnvMSYS2BinPath%\git.exe" %*
exit /B %ERRORLEVEL%
