@echo off

setlocal

setlocal

:: Initialize environment
call "%~dp0env.bat"
if errorlevel 1 goto error_env
call "%EnvPath%\env.bat"
if errorlevel 1 goto error_env

if not exist "%EnvMSYS2Path%" echo Nothing to update. & exit /B 0

if exist "%EnvMSYS2Path%\msys32" call :update 32
if exist "%EnvMSYS2Path%\msys64" call :update 64

goto :EOF

:update
set MSYS2SH=%EnvMSYS2Path%\msys%~1\usr\bin\sh

echo Update MSYS2 %~1
"%MSYS2SH%" -lc "yes | pacman --noconfirm -Syuu msys2-keyring"
"%MSYS2SH%" -lc "pacman --noconfirm -Su"

:exit
endlocal
goto :EOF

:error_env
echo Failed to initialize environment.
exit /B 1
