using System;
using Combo.BuildTools;

public class assimp : BuildTarget {
    public void Build() {
        this.SetName("assimp");
        this.SetBuildType(BuildType.Library);
        this.AddSourceFolder("Source");
        this.AddLibraryDirectory("Library");
    }
}