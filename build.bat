@echo off
set VERSION=0.0.2
set ZIP=buildtool\7za920\7za.exe
set WORKDIR=jrasm-%VERSION%
set ARCFILE=jrasm-%VERSION%.zip
msbuild jrasm.sln /clp:DisableConsoleColor /t:Rebuild /p:Configuration="Release" /p:Platform=x64
mkdir %WORKDIR%
copy x64\Release\jrasm.exe %WORKDIR%
copy README.md %WORKDIR%
if exist %ARCFILE% del /f %ARCFILE%
%ZIP% a %ARCFILE% %WORKDIR%\
rmdir /s /q %WORKDIR%
