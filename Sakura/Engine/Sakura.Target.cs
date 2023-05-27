using System;
using Sakura.BuildTools;

public class SakuraEngine : BuildTarget {
    public void Build() {
        this.SetName("Sakura.Engine");
        this.AddSourceFolder("Source");
        this.AddIncludeDirectory("Include");
        this.AddIncludeDirectory("Source");
        this.AddLibraryDirectory("Library");
        this.SetBuildType(BuildType.Executable);


        this.LinkLibrary("OpenGL32.lib");
    }
}