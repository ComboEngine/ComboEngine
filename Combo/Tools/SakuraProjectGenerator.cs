using System;
using System.IO;
using Newtonsoft.Json;

namespace Combo.Tools {
    public class ComboProjectGenerator {
        public static void Setup(string name) {
            Directory.CreateDirectory(name);
            Directory.CreateDirectory(name + "/" + "Content");
            Directory.CreateDirectory(name + "/" + "Source");
            Directory.CreateDirectory(name + "/" + "Cache");

            File.WriteAllText(name + "/" + name + ".Target.cs","using System;\n" +
                "using Combo.BuildTools;\n" +
                "\n" +
                "public class " + name + " : BuildTarget {\n" +
                "    public void Build() {\n" +
                "        this.SetName(\"" + name +  "\");\n" +
                "        this.SetBuildType(BuildType.Project);\n" +
                "        this.AddSourceFolder(\"Source\");\n" +
                "    }\n" +
                "}");

            Project project = new Project();
            project.name = name;
            project.language = "lua";

            File.WriteAllText(name + "/" + name + ".Comboproj",JsonConvert.SerializeObject(project));

        }
    }

    public class Project {
        public string name = "";
        public string language = "";
        public string cacheShaderPack = "Cache/ShaderPack.Combo";
        public string cacheAssetPack = "Cache/AssetPack.Combo";
    }
}