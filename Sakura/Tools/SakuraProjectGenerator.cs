using System;
using System.IO;
using Newtonsoft.Json;

namespace Sakura.Tools {
    public class SakuraProjectGenerator {
        public static void Setup(string name) {
            Directory.CreateDirectory(name);
            Directory.CreateDirectory(name + "/" + "Content");
            Directory.CreateDirectory(name + "/" + "Source");
            Directory.CreateDirectory(name + "/" + "Cache");

            File.WriteAllText(name + "/" + name + ".Target.cs","using System;\n" +
                "using Sakura.BuildTools;\n" +
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

            File.WriteAllText(name + "/" + name + ".sakuraproj",JsonConvert.SerializeObject(project));

        }
    }

    public class Project {
        public string name = "";
        public string language = "";
        public string cacheShaderPack = "Cache/ShaderPack.sakura";
        public string cacheAssetPack = "Cache/AssetPack.sakura";
    }
}