using System;
using System.Collections.Generic;
using Sakura.BuildTools;

public class SakuraEngine : BuildTarget {
    public void Build() {
        this.SetName("Sakura.Engine");
        this.AddSourceFolder("Source");
        this.AddIncludeDirectory("Source");
        this.SetBuildType(BuildType.Executable);

        List<string> libraries = new List<string>();
        libraries.Add("DirectX");
        libraries.Add("Lua");

        foreach(string library in libraries) {
            this.AddIncludeDirectory("Include\\" + library);
            this.AddLibraryDirectory("Library\\" + library);
        }


    }
}