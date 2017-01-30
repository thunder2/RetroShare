:: Initialize environment
call "%~dp0..\env.bat"
if errorlevel 1 goto error_env

set EnvRootPath=%RootPath%\%SourceName%-env
set EnvToolsPath=%EnvRootPath%\tools
set EnvTempPath=%EnvRootPath%\tmp
set EnvDownloadPath=%EnvRootPath%\download

::set EnvCurlExe=%EnvToolsPath%\curl.exe
set EnvWgetExe=%EnvToolsPath%\wget.exe
set EnvSevenZipExe=%EnvToolsPath%\7z.exe
set EnvSedExe=%EnvToolsPath%\sed.exe
set EnvCutExe=%EnvToolsPath%\cut.exe
set EnvDependsExe=%EnvToolsPath%\depends.exe
set EnvMakeNSISExe=%EnvToolsPath%\NSIS\makensis.exe
set EnvCEchoExe=%EnvToolsPath%\cecho.exe
set cecho=call "%ToolsPath%\cecho.bat"

:: Create folders
if not exist "%EnvRootPath%" mkdir "%EnvRootPath%"
if not exist "%EnvToolsPath%" mkdir "%EnvToolsPath%"
if not exist "%EnvDownloadPath%" mkdir "%EnvDownloadPath%"

call "%~dp0tools\prepare-tools.bat"
exit /B %ERRORLEVEL%

:error_env
echo Failed to initialize environment.
exit /B 1
