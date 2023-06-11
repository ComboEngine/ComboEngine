@echo off

rem Created by tecnessino
cd Combo
cd Tools
mkdir Binary
cd ..\..\
copy Combo\Tools\Libraries\Newtonsoft.Json.dll Combo\Tools\Binary\Newtonsoft.Json.dll
mcs Combo\Tools\**.cs -out:Combo\Tools\Binary\Combo.Tools.exe -r:Microsoft.Build.dll -r:Combo\Tools\Libraries\Newtonsoft.Json.dll