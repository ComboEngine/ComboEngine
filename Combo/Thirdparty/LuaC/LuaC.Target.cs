using System;
using Combo.BuildTools;

public class LuaC : BuildTarget {
    public void Build() {
        this.SetName("LuaC");
        this.SetBuildType(BuildType.Library);
        this.AddSourceFolder("Source");
        this.AddLibraryDirectory("Library");
    }
}