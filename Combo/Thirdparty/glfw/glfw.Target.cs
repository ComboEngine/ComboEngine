using System;
using Combo.BuildTools;

public class glfw : BuildTarget {
    public void Build() {
        this.SetName("glfw");
        this.SetBuildType(BuildType.Library);
        this.AddSourceFolder("Source");
        this.AddLibraryDirectory("Library");
    }
}