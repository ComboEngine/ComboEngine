using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using Newtonsoft.Json;
using Sakura.BuildTools;
using System.IO;

public class Settings {
    public string GraphicsAPI;
}

public class SakuraEngine : BuildTarget {
    public void Build() {
        this.SetName("Sakura.Engine");
        this.AddSourceFolder("Source");
        this.AddIncludeDirectory("Source");
        this.SetBuildType(BuildType.Executable);
        this.AddModule("Sakura.Lua");

        List<string> libraries = new List<string>();

        Settings json = JsonConvert.DeserializeObject<Settings>(File.ReadAllText("Sakura/Engine/Settings.json"));

        if(json.GraphicsAPI == "DirectX11") {
            libraries.Add("DirectX");
            AddDefinition("COMBO_DIRECTX11");
        }


        if(RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            AddDefinition("COMBO_WIN32");
            AddDefinition("COMBO_WINAPI");
        }
        if(RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
            AddDefinition("COMBO_MACOS");
        if(RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
            AddDefinition("COMBO_LINUX");

        Libraries.Add("Lua");
        Libraries.Add("assimp");
        Libraries.Add("glm");


        foreach(string library in Libraries) {
            AddIncludeDirectory("Include\\" + library);
            AddLibraryDirectory("Library\\" + library);
        }

    }
}