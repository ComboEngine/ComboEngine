using System;
using Sakura.BuildTools;

public class Lua : BuildTarget {
    public void Build() {
        this.SetName("Sakura.Lua");
        this.SetBuildType(BuildType.Project);
        this.AddSourceFolder("Source");
    }
}