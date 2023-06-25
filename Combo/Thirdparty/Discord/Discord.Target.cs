using System;
using Combo.BuildTools;

public class Discord : BuildTarget {
    public void Build() {
        this.SetName("Discord");
        this.SetBuildType(BuildType.Library);
        this.AddSourceFolder("Source");
        this.AddLibraryDirectory("Library");
    }
}