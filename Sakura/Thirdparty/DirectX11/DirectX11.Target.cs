using System;
using Sakura.BuildTools;

public class DirectX11 : BuildTarget {
    public void Build() {
        this.SetName("DirectX11");
        this.SetBuildType(BuildType.Library);
        this.AddSourceFolder("Source");
        this.AddLibraryDirectory("Library");
    }
}