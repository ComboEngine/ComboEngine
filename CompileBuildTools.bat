@echo off

rem Created by tecnessino
cd Sakura
cd Tools
mkdir Binary
cd ..\..\
copy Sakura\Tools\Libraries\Newtonsoft.Json.dll Sakura\Tools\Binary\Newtonsoft.Json.dll
mcs Sakura\Tools\**.cs -out:Sakura\Tools\Binary\Sakura.Tools.exe -r:Microsoft.Build.dll -r:Sakura\Tools\Libraries\Newtonsoft.Json.dll