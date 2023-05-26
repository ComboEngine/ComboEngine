using System;
using Sakura.BuildTools;

public class SakuraEngine : BuildTarget {
    public void Build() {
        this.SetName("Sakura.Engine");
        this.AddSourceFolder("Source");
        this.AddIncludeDirectory("Include");
        this.AddLibraryDirectory("Library");


        this.LinkLibrary("OpenGL32.lib");
    }
}