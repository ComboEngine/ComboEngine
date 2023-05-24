@echo off
mkdir Binary
cd Binary
mkdir Astur.Tools
cd ..\
mcs Astur.Tools\*.cs -out:Binary\Astur.Tools\Astur.Tools.exe