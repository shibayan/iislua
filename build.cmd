@echo off

set PATH=%PATH%;C:\Program Files (x86)\MSBuild\14.0\Bin

msbuild src\iislua.sln /p:Platform=Win32 /p:Configuration=Release /m
msbuild src\iislua.sln /p:Platform=x64 /p:Configuration=Release /m