:: Initialize environment
call "%~dp0..\env.bat"
if errorlevel 1 goto error_env

set EnvRootPath=%RootPath%\%SourceName%-env
set EnvToolsPath=%EnvRootPath%\tools
set EnvTempPath=%EnvRootPath%\tmp
set EnvDownloadPath=%EnvRootPath%\download
set EnvMSYS2Path=%EnvRootPath%\msys2

set EnvSevenZipExe=%EnvToolsPath%\7z.exe
set EnvMakeNSISExe=%EnvToolsPath%\NSIS\makensis.exe
set EnvCEchoExe=%EnvToolsPath%\cecho.exe
set cecho=call "%ToolsPath%\cecho.bat"

:: Create folders
if not exist "%EnvRootPath%" mkdir "%EnvRootPath%"
if not exist "%EnvToolsPath%" mkdir "%EnvToolsPath%"
if not exist "%EnvDownloadPath%" mkdir "%EnvDownloadPath%"

call "%~dp0tools\prepare-tools.bat"
if errorlevel 1 exit /B %ERRORLEVEL%

:: Add MinGit to PATH
set PATH=%EnvToolsPath%\MinGit\cmd;%EnvToolsPath%\cmake\bin;%PATH%
set HOME=%EnvToolsPath%\MinGit\home

:: Add Doxygen to PATH
set PATH=%EnvToolsPath%\doxygen;%PATH%

if not exist "%HOME%" mkdir "%HOME%"

exit /B 0

:error_env
echo Failed to initialize environment.
exit /B 1
