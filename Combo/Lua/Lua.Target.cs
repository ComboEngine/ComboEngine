using System;
using Combo.BuildTools;

public class Lua : BuildTarget {
    public void Build() {
        this.SetName("Combo.Lua");
        this.SetBuildType(BuildType.Project);
        this.AddSourceFolder("Source");
    }
}