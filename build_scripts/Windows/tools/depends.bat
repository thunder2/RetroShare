:: Usage:
:: call depends.bat [init^|list] file

if "%1"=="init" (
	:: Check MSYS environment
	if not exist "%EnvMSYS2SH%" %cecho% error "Please install MSYS2 first." & exit /B 1

	:: Install MSYS2 packages
	%EnvMSYS2Install% "mingw-w64-%MSYS2Architecture%-ntldd awk"
	if errorlevel 1 exit /B 1
	exit /B 0
)

if "%1" NEQ "list" goto usage
if "%2"=="" (
	goto usage
)

setlocal
pushd %~dp2

rem Don't use --recursive to not find false dependencies e.g. MSYS2 file when searching for MinGW files
%EnvMSYS2Cmd% "ntldd $0 | cut -f1 -d"=" | awk '{$1=$1};1'" %~nx2 > %~sdp0depends.tmp

for /F %%A in (%~sdp0depends.tmp) do (
	echo %%~A
)

if exist "%~dp0depends.tmp" del /Q "%~dp0depends.tmp"

popd
endlocal
exit /B 0

:usage
echo Usage: %~nx0 [init^|list] File
exit /B 1
