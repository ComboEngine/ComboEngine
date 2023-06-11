using System;
using Sakura.BuildTools;

public class glm : BuildTarget {
    public void Build() {
        this.SetName("glm");
        this.SetBuildType(BuildType.Library);
        this.AddSourceFolder("Source");
        this.AddLibraryDirectory("Library");
    }
}