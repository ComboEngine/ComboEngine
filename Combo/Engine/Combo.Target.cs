using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using Newtonsoft.Json;
using Combo.BuildTools;
using System.IO;

public class Settings {
    public string GraphicsAPI;
}

public class ComboEngine : BuildTarget {
    public void Build() {
        this.SetName("Combo.Engine");
        this.AddSourceFolder("Source");
        this.AddIncludeDirectory("Source");
        this.SetBuildType(BuildType.Library);
        this.AddModule("Combo.Lua");


        Settings json = JsonConvert.DeserializeObject<Settings>(File.ReadAllText("Combo/Engine/Settings.json"));

        if(json.GraphicsAPI == "DirectX11") {
            this.AddModule("DirectXTK");
            this.AddModule("glfw");
            AddDefinition("COMBO_DIRECTX11");
        }


        if(RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            AddDefinition("COMBO_BUILD_WIN32");
            AddDefinition("COMBO_BUILD_WINAPI");
        }
        if(RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
            AddDefinition("COMBO_BUILD_MACOS");
        if(RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
            AddDefinition("COMBO_BUILD_LINUX");

        this.AddModule("LuaC");
        this.AddModule("assimp");
        this.AddModule("glm");
        this.AddModule("ImGui");

    }
}