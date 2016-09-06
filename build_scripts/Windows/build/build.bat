@echo off

setlocal

:: Initialize environment
call "%~dp0..\env.bat"
if errorlevel 1 goto error_env
call "%EnvPath%\env.bat"
if errorlevel 1 goto error_env

:: Get gcc versions
call "%ToolsPath%\get-gcc-version.bat" GCCVersion
if "%GCCVersion%"=="" echo Cannot get gcc version.& exit /B 1

:: Check external libraries
if not exist "%RootPath%\libs" echo Please build external libraries first.& exit /B 1

:: Check gcc version of external libraries
if not exist "%RootPath%\libs\gcc-version" echo Cannot get gcc version of external libraries.& exit /B 1
set /P LibsGCCVersion=<"%RootPath%\libs\gcc-version"
if "%LibsGCCVersion%" NEQ "%GCCVersion%" echo Please use correct version of external libraries. (gcc %GCCVersion% ^<^> libs %LibsGCCVersion%).& exit /B 1

:: Initialize environment
call "%~dp0env.bat"
if errorlevel 1 goto error_env

if not exist "%RsBuildPath%" mkdir "%RsBuildPath%"
pushd "%RsBuildPath%"

:qmake
echo.
echo === qmake
echo.

title Build - %SourceName%-%RsBuildConfig% [qmake]

qmake "%SourcePath%\RetroShare.pro" -r "CONFIG+=%RsBuildConfig%"
if errorlevel 1 goto error

if exist "%SourcePath%\retroshare-nogui\src\rpc\proto\gencc\chat.pb.cc" goto protobuf_exists

echo.
echo === protobuf files
echo.

title Build - %SourceName%-%RsBuildConfig% [protobuf files]

pushd "%RsBuildPath%\retroshare-nogui\src"

mingw32-make -f Makefile.retroshare-nogui protobuf_gen
if errorlevel 1 goto error

if not exist "%SourcePath%\retroshare-nogui\src\rpc\proto\gencc\chat.pb.cc" echo Error generating protobuf files.&& goto error

popd

goto qmake

:protobuf_exists

echo.
echo === make
echo.

title Build - %SourceName%-%RsBuildConfig% [make]

mingw32-make

:error
popd

title %COMSPEC%

if errorlevel 1 echo.& echo Build failed& echo.
exit /B %ERRORLEVEL%

:error_env
echo Failed to initialize environment.
endlocal
exit /B 1
