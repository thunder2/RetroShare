@echo off

setlocal

:: Initialize environment
call "%~dp0..\env.bat"
if errorlevel 1 goto error_env
call "%EnvPath%\env.bat"
if errorlevel 1 goto error_env

:: Initialize environment
call "%~dp0env.bat" build %*
if errorlevel 2 exit /B 2
if errorlevel 1 goto error_env

:: Check external libraries
if not exist "%BuildLibsPath%\libs" %cecho% error "Please build external libraries first." & exit /B 1

:: Check gcc version of external libraries
if not exist "%BuildLibsPath%\libs\gcc-version" %cecho% error "Cannot get gcc version of external libraries." & exit /B 1
set /P LibsGCCVersion=<"%BuildLibsPath%\libs\gcc-version"
if "%LibsGCCVersion%" NEQ "%GCCVersion%" %cecho% error "Please use correct version of external libraries. (gcc %GCCVersion% ^<^> libs %LibsGCCVersion%)." & exit /B 1

:: Check git executable
::set GitPath=
::call "%ToolsPath%\find-in-path.bat" GitPath git.exe
::if "%GitPath%"=="" (
::	%cecho% error "Git not found in PATH. Version information cannot be determined."
::	exit /B 1
::)

echo.
echo === Version
echo.

title Build - %SourceName%-%RsBuildConfig% [Version]

pushd "%SourcePath%\retroshare-gui\src\gui\images"
:: Touch resource file
copy /b retroshare_win.rc +,,
popd

if not exist "%RsBuildPath%" mkdir "%RsBuildPath%"
pushd "%RsBuildPath%"

if exist "%EnvMSYS2SH%" (
	if exist "%EnvMSYS2SH%.bak" del /Q "%EnvMSYS2SH%.bak"
	ren "%EnvMSYS2SH%" sh.exe.bak
)

echo.
echo === qmake
echo.

title Build - %SourceName%-%RsBuildConfig% [qmake]

set RS_QMAKE_CONFIG=%RsBuildConfig%
if "%ParamAutologin%"=="1" set RS_QMAKE_CONFIG=%RS_QMAKE_CONFIG% rs_autologin
if "%ParamJsonApi%"=="1" set RS_QMAKE_CONFIG=%RS_QMAKE_CONFIG% rs_jsonapi
if "%ParamWebui%"=="1" set RS_QMAKE_CONFIG=%RS_QMAKE_CONFIG% rs_webui
if "%ParamPlugins%"=="1" set RS_QMAKE_CONFIG=%RS_QMAKE_CONFIG% retroshare_plugins
if "%ParamUseNativeDialogs%"=="1" set RS_QMAKE_CONFIG=%RS_QMAKE_CONFIG% rs_use_native_dialogs
if "%ParamService%" NEQ "1" set RS_QMAKE_CONFIG=%RS_QMAKE_CONFIG% no_retroshare_service
if "%ParamFriendServer%" NEQ "1" set RS_QMAKE_CONFIG=%RS_QMAKE_CONFIG% no_rs_friendserver
if "%ParamEmbeddedFriendServer%"=="1" set RS_QMAKE_CONFIG=%RS_QMAKE_CONFIG% rs_efs

qmake "%SourcePath%\RetroShare.pro" -r -spec win32-g++ "CONFIG+=%RS_QMAKE_CONFIG%" "EXTERNAL_LIB_DIR=%BuildLibsPath%\libs"
if errorlevel 1 goto error

echo.
echo === make
echo.

title Build - %SourceName%-%RsBuildConfig% [make]

mingw32-make -j %CoreCount%
if errorlevel 1 goto error

:: Webui
if "%ParamWebui%"=="1" (
	call :build-webui
	if errorlevel 1 goto error
) else (
	if exist "%RsWebuiBuildPath%" call "%ToolsPath%\remove-dir.bat" "%RsWebuiBuildPath%"
)

echo.
echo === Changelog
echo.

title Build - %SourceName%-%RsBuildConfig% [changelog]
call "%ToolsPath%\generate-changelog.bat" "%SourcePath%" "%RsBuildPath%\changelog.txt"

ren "%EnvMSYS2SH%.bak" sh.exe

:error
popd

title %COMSPEC%

if errorlevel 1 %cecho% error "\nBuild failed\n"
exit /B %ERRORLEVEL%

:error_env
echo Failed to initialize environment.
endlocal
exit /B 1

:build-webui
echo.
echo === webui
echo.
title Build webui

if not exist "%RsWebuiPath%" (
	echo Checking out webui source into %RsWebuiPath%
	git clone https://github.com/RetroShare/RSNewWebUI.git "%RsWebuiPath%"
	if errorlevel 1 exit /B 1
) else (
	echo Webui source found at %RsWebuiPath%
	pushd "%RsWebuiPath%"
	git pull
	popd
	if errorlevel 1 exit /B 1
)

pushd "%RsWebuiPath%\webui-src\make-src"
call build.bat
popd
if errorlevel 1 exit /B 1

if not exist "%RsWebuiPath%\webui" (
	%cecho% error "Webui is enabled, but no webui data found at %RsWebuiPath%\webui"
	exit /B 1
)

if exist "%RsWebuiBuildPath%" call "%ToolsPath%\remove-dir.bat" "%RsWebuiBuildPath%"
move "%RsWebuiPath%\webui" "%RsWebuiBuildPath%"
if errorlevel 1 exit /B 1

exit /B 0
