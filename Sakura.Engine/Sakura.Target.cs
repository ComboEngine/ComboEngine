using System;
using Sakura.BuildTools;

public class SakuraEngine : BuildTarget {
    public BuildInfo build() {
        BuildInfo info = new BuildInfo();
        info.SetName("Sakura.Engine");
        return info;
    }
}