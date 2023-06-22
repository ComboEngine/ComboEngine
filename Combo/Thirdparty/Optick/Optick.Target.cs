using System;
using Combo.BuildTools;

public class Optick : BuildTarget {
    public void Build() {
        this.SetName("Optick");
        this.SetBuildType(BuildType.Library);
        this.AddSourceFolder("Source");
        this.AddLibraryDirectory("Library");
    }
}