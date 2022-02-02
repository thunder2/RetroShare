@echo off

setlocal

:: Initialize environment
call "%~dp0env.bat"
if errorlevel 1 goto error_env
call "%EnvPath%\env.bat"
if errorlevel 1 goto error_env

%cecho% info "Update MSYS2"
call "%~dp0update-msys2.bat"
if errorlevel 1 %cecho% error "Failed to update MSYS2." & exit /B %ERRORLEVEL%

%cecho% info "Build libraries"
call "%~dp0build-libs\build-libs.bat"
if errorlevel 1 %cecho% error "Failed to build libraries." & exit /B %ERRORLEVEL%

%cecho% info "Build %SourceName%"
call "%~dp0build\build.bat" release autologin jsonapi plugins nativedialogs service
if errorlevel 1 %cecho% error "Failed to build %SourceName%." & exit /B %ERRORLEVEL%

%cecho% info "Pack %SourceName%"
call "%~dp0build\pack.bat" release plugins service
if errorlevel 1 %cecho% error "Failed to pack %SourceName%." & exit /B %ERRORLEVEL%
call "%~dp0build\pack.bat" release plugins service tor
if errorlevel 1 %cecho% error "Failed to pack %SourceName%." & exit /B %ERRORLEVEL%

%cecho% info "Build installer"
call "%~dp0build\build-installer.bat"
if errorlevel 1 %cecho% error "Failed to build installer." & exit /B %ERRORLEVEL%

exit /B 0

:error_env
echo Failed to initialize environment.
exit /B 1
