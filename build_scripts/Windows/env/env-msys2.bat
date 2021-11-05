:: Usage:
:: call env-msys2.bat [reinstall|clean]

:: Initialize environment
call "%~dp0env.bat"
if errorlevel 1 goto error_env

:: Get gcc versions
call "%ToolsPath%\get-gcc-version.bat" GCCVersion GCCArchitecture
if "%GCCVersion%"=="" %cecho% error "Cannot get gcc version." & exit /B 1
if "%GCCArchitecture%"=="" %cecho% error "Cannot get gcc architecture." & exit /B 1

if "%GCCArchitecture%"=="x64" (
	:: x64
	set MSYS2Architecture=x86_64
	set MSYS2Base=64
) else (
	:: x86
	set MSYS2Architecture=i686
	set MSYS2Base=32
)

set CHERE_INVOKING=1
set MSYSTEM=MINGW%MSYS2Base%
set MSYS2_PATH_TYPE=inherit

set EnvMSYS2BasePath=%EnvMSYS2Path%\msys%MSYS2Base%
set EnvMSYS2BinPath=%EnvMSYS2BasePath%\usr\bin

set EnvMSYS2MinGWPath=%EnvMSYS2BasePath%\mingw%MSYS2Base%
set EnvMSYS2MinGWBinPath=%EnvMSYS2MinGWPath%\bin

call "%~dp0tools\prepare-msys2.bat" %1
if errorlevel 1 exit /B 1

set EnvMSYS2SH=%EnvMSYS2BinPath%\sh.exe
if not exist "%EnvMSYS2SH%" if errorlevel 1 goto error_env

set EnvMSYS2Cmd="%EnvMSYS2SH%" -lc
set EnvMSYS2Install=%EnvMSYS2Cmd% "pacman --needed --noconfirm -S $0"

rem Don't add MSYS2 to path to prevent Qt from finding sh.exe and set QMAKE_SH
set PATH=%EnvMSYS2BinPath%;%EnvMSYS2BasePath%\usr\lib\git-core;%PATH%

exit /B 0

:error_env
echo Failed to initialize environment.
exit /B 1
