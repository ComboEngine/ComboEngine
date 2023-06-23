using System;
using Combo.BuildTools;

public class SmallLibraries : BuildTarget {
    public void Build() {
        this.SetName("SmallLibraries");
        this.SetBuildType(BuildType.Library);
        this.AddSourceFolder("Source");
        this.AddLibraryDirectory("Library");
    }
}