setlocal

if "%EnvRootPath%"=="" exit /B 1

set CEchoUrl=https://github.com/lordmulder/cecho/releases/download/2015-10-10/cecho.2015-10-10.zip
set CEchoInstall=cecho.2015-10-10.zip
set SevenZipUrl=https://sourceforge.net/projects/sevenzip/files/7-Zip/19.00/7z1900.msi/download
set SevenZipInstall=7z1900.msi
set MinGitInstall=MinGit-2.28.0-32-bit.zip
set MinGitUrl=https://github.com/git-for-windows/git/releases/download/v2.28.0.windows.1/%MinGitInstall%
set MinGitInstallPath=%EnvToolsPath%\MinGit
set DoxygenInstall=doxygen-1.9.4.windows.x64.bin.zip
set DoxygenUrl=https://www.doxygen.nl/files/%DoxygenInstall%
set DoxygenInstallPath=%EnvToolsPath%\doxygen
set CMakeVersion=cmake-3.19.0-win32-x86
set CMakeInstall=%CMakeVersion%.zip
set CMakeUrl=https://github.com/Kitware/CMake/releases/download/v3.19.0/%CMakeInstall%
set CMakeInstallPath=%EnvToolsPath%\cmake

if not exist "%EnvToolsPath%\7z.exe" (
	call "%ToolsPath%\remove-dir.bat" "%EnvTempPath%"
	mkdir "%EnvTempPath%"

	echo Download 7z installation

	if not exist "%EnvDownloadPath%\%SevenZipInstall%" call "%ToolsPath%\download-file.bat" "%SevenZipUrl%" "%EnvDownloadPath%\%SevenZipInstall%"
	if not exist "%EnvDownloadPath%\%SevenZipInstall%" echo Cannot download 7z installation& goto error

	echo Unpack 7z
	msiexec /a "%EnvDownloadPath%\%SevenZipInstall%" /qb TARGETDIR="%EnvTempPath%"
	copy "%EnvTempPath%\Files\7-Zip\7z.dll" "%EnvToolsPath%"
	copy "%EnvTempPath%\Files\7-Zip\7z.exe" "%EnvToolsPath%"

	call "%ToolsPath%\remove-dir.bat" "%EnvTempPath%"
)

if not exist "%EnvToolsPath%\cecho.exe" (
	call "%ToolsPath%\remove-dir.bat" "%EnvTempPath%"
	mkdir "%EnvTempPath%"

	echo Download cecho installation

	if not exist "%EnvDownloadPath%\%CEchoInstall%" call "%ToolsPath%\download-file.bat" "%CEchoUrl%" "%EnvDownloadPath%\%CEchoInstall%"
	if not exist "%EnvDownloadPath%\%cCEhoInstall%" echo Cannot download cecho installation& goto error

	echo Unpack cecho
	"%EnvSevenZipExe%" x -o"%EnvTempPath%" "%EnvDownloadPath%\%CEchoInstall%"
	copy "%EnvTempPath%\cecho.exe" "%EnvToolsPath%"

	call "%ToolsPath%\remove-dir.bat" "%EnvTempPath%"
)

if not exist "%MinGitInstallPath%\cmd\git.exe" (
	%cecho% info "Download MinGit installation"

	if not exist "%EnvDownloadPath%\%MinGitInstall%" call "%ToolsPath%\download-file.bat" "%MinGitUrl%" "%EnvDownloadPath%\%MinGitInstall%"
	if not exist "%EnvDownloadPath%\%MinGitInstall%" %cecho% error "Cannot download MinGit installation" & goto error

	%cecho% info "Unpack MinGit"
	"%EnvSevenZipExe%" x -o"%MinGitInstallPath%" "%EnvDownloadPath%\%MinGitInstall%"
)

if not exist "%EnvDownloadPath%\%DoxygenInstall%" call "%ToolsPath%\remove-dir.bat" "%DoxygenInstallPath%"
if not exist "%DoxygenInstallPath%\doxygen.exe" (
	if exist "%DoxygenInstallPath%" call "%ToolsPath%\remove-dir.bat" "%DoxygenInstallPath%"

	%cecho% info "Download Doxygen installation"

	if not exist "%EnvDownloadPath%\%DoxygenInstall%" call "%ToolsPath%\download-file.bat" "%DoxygenUrl%" "%EnvDownloadPath%\%DoxygenInstall%"
	if not exist "%EnvDownloadPath%\%DoxygenInstall%" %cecho% error "Cannot download doxygen installation" & goto error

	%cecho% info "Unpack Doxygen"
	"%EnvSevenZipExe%" x -o"%DoxygenInstallPath%" "%EnvDownloadPath%\%DoxygenInstall%"
)

if not exist "%EnvDownloadPath%\%CMakeInstall%" call "%ToolsPath%\remove-dir.bat" "%CMakeInstallPath%"
if not exist "%CMakeInstallPath%\bin\cmake.exe" (
	%cecho% info "Download CMake installation"

	if exist "%CMakeInstallPath%" call "%ToolsPath%\remove-dir.bat" "%CMakeInstallPath%"

	mkdir "%EnvTempPath%"

	if not exist "%EnvDownloadPath%\%CMakeInstall%" call "%ToolsPath%\download-file.bat" "%CMakeUrl%" "%EnvDownloadPath%\%CMakeInstall%"
	if not exist "%EnvDownloadPath%\%CMakeInstall%" %cecho% error "Cannot download CMake installation" & goto error

	%cecho% info "Unpack CMake"
	"%EnvSevenZipExe%" x -o"%EnvTempPath%" "%EnvDownloadPath%\%CMakeInstall%"

	move "%EnvTempPath%\%CMakeVersion%" "%CMakeInstallPath%"

	call "%ToolsPath%\remove-dir.bat" "%EnvTempPath%"
)

:exit
endlocal
exit /B 0

:error
call "%ToolsPath%\remove-dir.bat" "%EnvTempPath%"
endlocal
exit /B 1
