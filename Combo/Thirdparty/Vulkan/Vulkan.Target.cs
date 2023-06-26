using System;
using Combo.BuildTools;

public class Vulkan : BuildTarget {
    public void Build() {
        this.SetName("Vulkan");
        this.SetBuildType(BuildType.Library);
        this.AddSourceFolder("Source");
        this.AddLibraryDirectory("Library");
    }
}