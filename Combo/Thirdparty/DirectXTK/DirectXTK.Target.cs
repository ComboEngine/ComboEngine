using System;
using Combo.BuildTools;

public class DirectXTK : BuildTarget {
    public void Build() {
        this.SetName("DirectXTK");
        this.SetBuildType(BuildType.Library);
        this.AddSourceFolder("Source");
        this.AddLibraryDirectory("Library");
    }
}