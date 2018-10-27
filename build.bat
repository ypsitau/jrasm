@echo off
set VERSION=0.3.0
set ZIP=src\guest\buildtool\7za920\7za.exe
set WORKDIR=jrasm-%VERSION%
set ARCFILE=jrasm-%VERSION%.zip
if "%1" == "release" (
	msbuild src\jrasm.sln /clp:DisableConsoleColor /t:Rebuild /p:Configuration="Release" /p:Platform=x86
	if exist %WORKDIR% rmdir /s /q %WORKDIR%
	mkdir %WORKDIR%
	copy src\guest\VC_redist\*.dll %WORKDIR%
	copy src\Release\jrasm.exe %WORKDIR%
	copy README.md %WORKDIR%
	if exist %ARCFILE% del /f %ARCFILE%
	%ZIP% a %ARCFILE% %WORKDIR%\
	rmdir /s /q %WORKDIR%
) else (
	msbuild src\jrasm.sln /clp:DisableConsoleColor /p:Configuration="Release" /p:Platform=x86
	copy src\Release\jrasm.exe
)
