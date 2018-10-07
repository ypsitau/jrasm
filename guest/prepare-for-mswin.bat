@echo off
rem ---------------------------------------------------------------------------
rem Main entry
rem ---------------------------------------------------------------------------
:main
set GUESTURL=http://www.gura-lang.org/guest
set BASEDIR=%~dp0
set UNZIP="%BASEDIR%buildtool\7za920\7za.exe"
set CURL="%BASEDIR%buildtool\curl\curl.exe"
if not defined VCINSTALLDIR call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\vsdevcmd.bat" -arch=x86
rem Add include path containing Win32.mak in case vs2015 doesn't include SDK providing that file.
set INCLUDE=%BASEDIR%include;%INCLUDE%
rem ---------------------------------------------------------------------------
rem Build specified package
rem ---------------------------------------------------------------------------
if not "%1" == "" (
	set PACKAGES=
	call :getinfo_%1
	call :download_packages
	call :clean_%1
	call :build_%1
	exit /b
)
rem ---------------------------------------------------------------------------
rem Register labels of subroutines
rem ---------------------------------------------------------------------------
set LABELS=wx
rem ---------------------------------------------------------------------------
rem Download packages
rem ---------------------------------------------------------------------------
set PACKAGES=
for %%L in (%LABELS%) do call :getinfo_%%L
call :download_packages
rem ---------------------------------------------------------------------------
rem Build packages
rem ---------------------------------------------------------------------------
set FAILEDLIST=
for %%L in (%LABELS%) do call :build_%%L
if not "%FAILEDLIST%" == "" (
	echo ======================================================================
	echo Error occured:%FAILEDLIST%
	echo ======================================================================
)
pause
exit /b

rem ---------------------------------------------------------------------------
rem Download packages
rem ---------------------------------------------------------------------------
:download_packages
for %%P in (%PACKAGES%) do (
	echo %%P
	if not exist %%P %CURL% -O %GUESTURL%/%%P
)
exit /b

rem ---------------------------------------------------------------------------
rem Subroutines
rem ---------------------------------------------------------------------------

rem ---------------------------------------------------------------------------
:getinfo_wx
set VER_wx=3.1.0
set PACKAGES=%PACKAGES% wxWidgets-%VER_wx%.7z
set PACKAGES=%PACKAGES% wxWidgets-%VER_wx%-gurapatch-vs2017.zip
exit /b

:clean_wx
rmdir /s /q wxWidgets-%VER_wx%
exit /b

:build_wx
rem Building wxWidgets library using /m option doesn't produce correct results.
rem It seems that wxWidgets-3.1.1 has some bugs in handling image list.
%UNZIP% x -y -owxWidgets-%VER_wx% wxWidgets-%VER_wx%.7z
%UNZIP% x -y wxWidgets-%VER_wx%-gurapatch-vs2017.zip
msbuild wxWidgets-%VER_wx%\build\msw\wx_vc14.sln /clp:DisableConsoleColor /t:Build /p:Configuration=Release /p:Platform=win32
if ERRORLEVEL 1 set FAILEDLIST=%FAILEDLIST% wx
exit /b
