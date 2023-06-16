using System;
using Combo.BuildTools;

public class ImGui : BuildTarget
{
    public void Build()
    {
        this.SetName("ImGui");
        this.SetBuildType(BuildType.Library);
        this.AddSourceFolder("Source");
        this.AddLibraryDirectory("Library");
    }
}