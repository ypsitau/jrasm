@echo off
msbuild jrasm.sln /clp:DisableConsoleColor /t:Rebuild /p:Configuration="Release" /p:Platform=x64
