using System;
using Sakura.BuildTools;

public class Sandbox : BuildTarget {
    public void Build() {
        this.SetName("Sandbox");
        this.AddSourceFolder("Source");
        this.AddModule("Sakura.Engine");
        this.SetBuildType(BuildType.Executable);
    }
}