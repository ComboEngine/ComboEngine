@echo off
mkdir Binary
cd Binary
mkdir Sakura.Tools
cd ..\
mcs Sakura.Tools\*.cs -out:Binary\Sakura.Tools\Sakura.Tools.exe