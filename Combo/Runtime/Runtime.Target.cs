using System;
using Combo.BuildTools;

public class Runtime : BuildTarget {
    public void Build() {
        this.SetName("Runtime");
        this.SetLanguage(Language.CSharp);
        this.SetBuildType(BuildType.Executable);
        this.AddSourceFolder("Source");
    }
}