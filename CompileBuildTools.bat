@echo off

rem Created by tecnessino
cd Sakura
cd Tools
mkdir Binary
cd ..\..\
mcs Sakura\Tools\**.cs -out:Sakura\Tools\Binary\Sakura.Tools.exe -r:Microsoft.Build.dll