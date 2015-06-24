@echo off

msbuild src\iislua.sln /p:Platform=Win32 /p:Configuration=Release /m
msbuild src\iislua.sln /p:Platform=x64 /p:Configuration=Release /m