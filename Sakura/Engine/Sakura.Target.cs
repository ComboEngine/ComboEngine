using System;
using System.Collections.Generic;
using Sakura.BuildTools;

public class SakuraEngine : BuildTarget {
    public void Build() {
        this.SetName("Sakura.Engine");
        this.AddSourceFolder("Source");
        this.AddIncludeDirectory("Source");
        this.SetBuildType(BuildType.Executable);
        this.AddModule("Sakura.Lua");

        List<string> libraries = new List<string>();
        libraries.Add("DirectX");
        libraries.Add("Lua");
        libraries.Add("LZ4");
        libraries.Add("assimp");

        foreach(string library in libraries) {
            this.AddIncludeDirectory("Include\\" + library);
            this.AddLibraryDirectory("Library\\" + library);
        }


    }
}